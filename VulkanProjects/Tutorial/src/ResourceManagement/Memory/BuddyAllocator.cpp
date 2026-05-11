#include "ResourceManagement/Memory/BuddyAllocator.h"

#include <cstring>
#include <stdexcept>

namespace Tutorial::ResourceManagement {
    namespace {
        constexpr std::size_t BITMAP_WORD_BITS = sizeof(std::uint64_t) * 8;

        bool pointerInRange(const void* ptr, const void* begin, const Bytes bytes) {
            const auto address = reinterpret_cast<std::uintptr_t>(ptr);
            const auto start = reinterpret_cast<std::uintptr_t>(begin);
            return address >= start && address < start + bytes.value();
        }
    }

    BuddyAllocator::BuddyAllocator(const Bytes arenaSize, const Bytes minBlockSize)
        : _bumpAllocator(arenaSize),
          _alignedAllocators(nullptr),
          _minBlockSize(roundUpToPowerOfTwo(maxBytes(minBlockSize, Bytes::fromSizeT(sizeof(FreeBlock))))),
          _maxOrder{0} {
        if (!isPowerOfTwo(arenaSize)) {
            throw std::invalid_argument("BuddyAllocator: arena size must be a power of two");
        }
        if (_minBlockSize > arenaSize) {
            throw std::invalid_argument("BuddyAllocator: min block size must not exceed arena size");
        }
        Bytes current = _minBlockSize;
        while (current < arenaSize) {
            current = current * 2;
            ++_maxOrder.value;
            if (_maxOrder.value >= 32) {
                throw std::invalid_argument("BuddyAllocator: too many buddy orders");
            }
        }
    }

    BuddyAllocator::AlignedBuddyAllocator* BuddyAllocator::getOrCreateAlignedAllocator(const Alignment alignment) {
        auto* current = _alignedAllocators;
        while (current != nullptr) {
            if (current->alignment == alignment) {
                return current;
            }
            current = current->next;
        }
        auto* created = new AlignedBuddyAllocator{alignment, nullptr, _alignedAllocators};
        _alignedAllocators = created;
        return created;
    }

    BuddyAllocator::ArenaState* BuddyAllocator::createArena(AlignedBuddyAllocator& allocator) {
        auto* arena = _bumpAllocator.allocateArena(allocator.alignment);
        auto* state = new ArenaState{};
        state->arena = arena;
        state->next = allocator.arenas;
        allocator.arenas = state;

        for (BuddyOrder order{0}; order <= _maxOrder; ++order.value) {
            state->freeLists[order.value] = nullptr;
            const std::size_t blockCount = getArenaSize().value() / bytesForOrder(order).value();
            const std::size_t wordCount = (blockCount + BITMAP_WORD_BITS - 1) / BITMAP_WORD_BITS;
            state->bitmapWordCounts[order.value] = wordCount;
            state->bitmaps[order.value] = new std::uint64_t[wordCount]{};
        }
        pushFreeBlock(*state, _maxOrder, BuddyBlockIndex{0});
        return state;
    }

    BuddyAllocator::BuddyOrder BuddyAllocator::orderFor(const Bytes size) const {
        const Bytes rounded = roundUpToPowerOfTwo(maxBytes(size, _minBlockSize));
        if (rounded > getArenaSize()) {
            throw std::invalid_argument("BuddyAllocator: requested size exceeds arena size");
        }
        BuddyOrder order{0};
        Bytes current = _minBlockSize;
        while (current < rounded) {
            current = current * 2;
            ++order.value;
        }
        return order;
    }

    Bytes BuddyAllocator::bytesForOrder(const BuddyOrder order) const {
        Bytes result = _minBlockSize;
        for (std::size_t i = 0; i < order.value; ++i) {
            result = result * 2;
        }
        return result;
    }

    BuddyAllocator::ArenaState* BuddyAllocator::findArenaContaining(AlignedBuddyAllocator& allocator, void* ptr) const {
        auto* current = allocator.arenas;
        while (current != nullptr) {
            if (pointerInRange(ptr, current->arena->block.ptr, getArenaSize())) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    bool BuddyAllocator::isBlockFree(const ArenaState& arena, const BuddyOrder order, const BuddyBlockIndex index) const {
        const auto word = index.value / BITMAP_WORD_BITS;
        const auto bit = index.value % BITMAP_WORD_BITS;
        return (arena.bitmaps[order.value][word] & (std::uint64_t{1} << bit)) != 0;
    }

    void BuddyAllocator::setBlockFree(ArenaState& arena, const BuddyOrder order, const BuddyBlockIndex index, const bool value) const {
        const auto word = index.value / BITMAP_WORD_BITS;
        const auto bit = index.value % BITMAP_WORD_BITS;
        const auto mask = std::uint64_t{1} << bit;
        if (value) {
            arena.bitmaps[order.value][word] |= mask;
        } else {
            arena.bitmaps[order.value][word] &= ~mask;
        }
    }

    void BuddyAllocator::pushFreeBlock(ArenaState& arena, const BuddyOrder order, const BuddyBlockIndex index) {
        auto* block = static_cast<FreeBlock*>(ptrForIndex(arena, order, index));
        block->next = arena.freeLists[order.value];
        arena.freeLists[order.value] = block;
        setBlockFree(arena, order, index, true);
    }

    BuddyAllocator::FreeBlock* BuddyAllocator::removeFreeBlock(ArenaState& arena, const BuddyOrder order, const BuddyBlockIndex index) {
        FreeBlock* previous = nullptr;
        auto* current = arena.freeLists[order.value];
        void* expected = ptrForIndex(arena, order, index);
        while (current != nullptr) {
            if (current == expected) {
                if (previous == nullptr) {
                    arena.freeLists[order.value] = current->next;
                } else {
                    previous->next = current->next;
                }
                setBlockFree(arena, order, index, false);
                current->next = nullptr;
                return current;
            }
            previous = current;
            current = current->next;
        }
        return nullptr;
    }

    BuddyAllocator::BuddyBlockIndex BuddyAllocator::blockIndex(const ArenaState& arena, const void* ptr, const BuddyOrder order) const {
        const auto address = reinterpret_cast<std::uintptr_t>(ptr);
        const auto base = reinterpret_cast<std::uintptr_t>(arena.arena->block.ptr);
        const auto offset = address - base;
        const auto blockSize = bytesForOrder(order).value();
        if (offset % blockSize != 0) {
            throw std::invalid_argument("BuddyAllocator: pointer is not aligned to block order");
        }
        return BuddyBlockIndex{offset / blockSize};
    }

    void* BuddyAllocator::ptrForIndex(const ArenaState& arena, const BuddyOrder order, const BuddyBlockIndex index) const {
        const auto base = reinterpret_cast<std::uintptr_t>(arena.arena->block.ptr);
        return reinterpret_cast<void*>(base + bytesForOrder(order).value() * index.value);
    }

    AlignedContinuousMemoryBlock BuddyAllocator::allocate(const Bytes size, const Alignment alignment) {
        if (size.isZero()) {
            throw std::invalid_argument("BuddyAllocator: size must be greater than 0");
        }
        const BuddyOrder targetOrder = orderFor(maxBytes(size, alignment.bytes()));
        auto* allocator = getOrCreateAlignedAllocator(alignment);

        ArenaState* selectedArena = nullptr;
        BuddyOrder selectedOrder = targetOrder;
        for (auto* arena = allocator->arenas; arena != nullptr && selectedArena == nullptr; arena = arena->next) {
            for (BuddyOrder order = targetOrder; order <= _maxOrder; ++order.value) {
                if (arena->freeLists[order.value] != nullptr) {
                    selectedArena = arena;
                    selectedOrder = order;
                    break;
                }
            }
        }

        if (selectedArena == nullptr) {
            selectedArena = createArena(*allocator);
            selectedOrder = _maxOrder;
        }

        auto* block = selectedArena->freeLists[selectedOrder.value];
        const BuddyBlockIndex selectedIndex = blockIndex(*selectedArena, block, selectedOrder);
        if (removeFreeBlock(*selectedArena, selectedOrder, selectedIndex) == nullptr) {
            throw std::runtime_error("BuddyAllocator: selected free block disappeared");
        }

        while (selectedOrder > targetOrder) {
            --selectedOrder.value;
            const BuddyBlockIndex leftIndex = blockIndex(*selectedArena, block, selectedOrder);
            const BuddyBlockIndex rightIndex{leftIndex.value ^ 1};
            pushFreeBlock(*selectedArena, selectedOrder, rightIndex);
        }

        return AlignedContinuousMemoryBlock{block, bytesForOrder(targetOrder), alignment};
    }

    void BuddyAllocator::deallocate(const AlignedContinuousMemoryBlock block) {
        if (block.ptr == nullptr) {
            throw std::invalid_argument("BuddyAllocator: block ptr must not be null");
        }
        BuddyOrder order = orderFor(maxBytes(block.size, block.alignment.bytes()));
        auto* allocator = getOrCreateAlignedAllocator(block.alignment);
        auto* arena = findArenaContaining(*allocator, block.ptr);
        if (arena == nullptr) {
            throw std::invalid_argument("BuddyAllocator: block does not belong to this allocator");
        }

        BuddyBlockIndex index = blockIndex(*arena, block.ptr, order);
        while (order < _maxOrder) {
            const BuddyBlockIndex buddyIndex{index.value ^ 1};
            if (!isBlockFree(*arena, order, buddyIndex)) {
                break;
            }
            if (removeFreeBlock(*arena, order, buddyIndex) == nullptr) {
                throw std::runtime_error("BuddyAllocator: bitmap and free list are out of sync");
            }
            index.value /= 2;
            ++order.value;
        }
        pushFreeBlock(*arena, order, index);
    }

    Bytes BuddyAllocator::getArenaSize() const {
        return _bumpAllocator.getArenaSize();
    }

    void BuddyAllocator::destroyMetadata() noexcept {
        auto* aligned = _alignedAllocators;
        while (aligned != nullptr) {
            auto* nextAligned = aligned->next;
            auto* arena = aligned->arenas;
            while (arena != nullptr) {
                auto* nextArena = arena->next;
                for (BuddyOrder order{0}; order <= _maxOrder; ++order.value) {
                    delete[] arena->bitmaps[order.value];
                }
                delete arena;
                arena = nextArena;
            }
            delete aligned;
            aligned = nextAligned;
        }
        _alignedAllocators = nullptr;
    }

    BuddyAllocator::~BuddyAllocator() {
        destroyMetadata();
    }
}
