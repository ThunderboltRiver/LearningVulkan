#include "ResourceManagement/Memory/BuddyAlloc/AlignedBuddyAllocator.h"

#include "ResourceManagement/Memory/BuddyAlloc/BuddyFreeBitmap.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {
    namespace {
        constexpr std::size_t BITMAP_WORD_BITS = sizeof(std::uint64_t) * 8;

        bool pointerInRange(const void* ptr, const void* begin, const Bytes bytes) {
            const auto address = reinterpret_cast<std::uintptr_t>(ptr);
            const auto start = reinterpret_cast<std::uintptr_t>(begin);
            return address >= start && address < start + bytes.value();
        }

        AlignedContinuousMemoryBlock emptyBlock(const Alignment alignment) {
            return AlignedContinuousMemoryBlock{
                nullptr,
                Bytes::fromSizeT(0),
                alignment
            };
        }
    }

    AlignedBuddyAllocator::AlignedBuddyAllocator(const Alignment alignment, AlignedBuddyAllocator* next)
        : alignment(alignment), arenas(nullptr), next(next) {
    }

    AlignedBuddyAllocator::~AlignedBuddyAllocator() {
        auto* arena = arenas;
        while (arena != nullptr) {
            auto* nextArena = arena->next;
            for (std::size_t order = 0; order < BUDDY_ORDER_COUNT; ++order) {
                delete[] arena->freeBitmaps[order].words;
            }
            delete arena;
            arena = nextArena;
        }
        arenas = nullptr;
    }

    bool AlignedBuddyAllocator::satisfies(const Alignment requestedAlignment) const {
        return alignment.bytes().value() % requestedAlignment.bytes().value() == 0;
    }

    AlignedContinuousMemoryBlock AlignedBuddyAllocator::tryAllocate(
        const Bytes size,
        const BuddyOrder targetOrder,
        const Bytes minBlockSize,
        const BuddyOrder maxOrder
    ) {
        (void)size;
        if (targetOrder > maxOrder) {
            return emptyBlock(alignment);
        }
        return allocateFromExistingArena(size, targetOrder, minBlockSize, maxOrder);
    }

    AlignedContinuousMemoryBlock AlignedBuddyAllocator::allocateWithNewArena(
        const Bytes size,
        const BuddyOrder targetOrder,
        const Bytes minBlockSize,
        const BuddyOrder maxOrder,
        BumpAlloc::BumpAllocator& bumpAllocator
    ) {
        (void)createArena(maxOrder, minBlockSize, bumpAllocator);
        return allocateFromExistingArena(size, targetOrder, minBlockSize, maxOrder);
    }

    ArenaState* AlignedBuddyAllocator::createArena(
        const BuddyOrder maxOrder,
        const Bytes minBlockSize,
        BumpAlloc::BumpAllocator& bumpAllocator
    ) {
        auto* arena = bumpAllocator.allocateArena(alignment);
        auto* state = new ArenaState{};
        state->arena = arena;
        state->next = arenas;
        arenas = state;

        for (BuddyOrder order{0}; order <= maxOrder; ++order.value) {
            state->freeLists[order.value] = nullptr;
            const std::size_t blockCount = bumpAllocator.getArenaSize().value() / bytesForOrder(order, minBlockSize).value();
            const std::size_t wordCount = (blockCount + BITMAP_WORD_BITS - 1) / BITMAP_WORD_BITS;
            state->freeBitmaps[order.value] = BuddyFreeBitmap{
                new std::uint64_t[wordCount]{},
                wordCount
            };
        }

        pushFreeBlock(*state, maxOrder, BuddyBlockIndex{0}, minBlockSize);
        return state;
    }

    Bytes AlignedBuddyAllocator::bytesForOrder(const BuddyOrder order, const Bytes minBlockSize) {
        Bytes result = minBlockSize;
        for (std::size_t i = 0; i < order.value; ++i) {
            result = result * 2;
        }
        return result;
    }

    ArenaState* AlignedBuddyAllocator::findArenaContaining(void* ptr, const Bytes arenaSize) const {
        auto* current = arenas;
        while (current != nullptr) {
            if (pointerInRange(ptr, current->arena->block.ptr, arenaSize)) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    BuddyBlockIndex AlignedBuddyAllocator::blockIndex(
        const ArenaState& arena,
        const void* ptr,
        const BuddyOrder order,
        const Bytes minBlockSize
    ) {
        const auto address = reinterpret_cast<std::uintptr_t>(ptr);
        const auto base = reinterpret_cast<std::uintptr_t>(arena.arena->block.ptr);
        const auto offset = address - base;
        const auto blockSize = bytesForOrder(order, minBlockSize).value();
        if (offset % blockSize != 0) {
            throw std::invalid_argument("AlignedBuddyAllocator: pointer is not aligned to block order");
        }
        return BuddyBlockIndex{offset / blockSize};
    }

    void* AlignedBuddyAllocator::ptrForIndex(
        const ArenaState& arena,
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const Bytes minBlockSize
    ) {
        const auto base = reinterpret_cast<std::uintptr_t>(arena.arena->block.ptr);
        return reinterpret_cast<void*>(base + bytesForOrder(order, minBlockSize).value() * index.value);
    }

    void AlignedBuddyAllocator::setBlockFree(
        ArenaState& arena,
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const bool value
    ) {
        arena.freeBitmaps[order.value].setFree(index, value);
    }

    bool AlignedBuddyAllocator::isBlockFree(
        const ArenaState& arena,
        const BuddyOrder order,
        const BuddyBlockIndex index
    ) {
        return arena.freeBitmaps[order.value].isFree(index);
    }

    void AlignedBuddyAllocator::pushFreeBlock(
        ArenaState& arena,
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const Bytes minBlockSize
    ) {
        auto* block = static_cast<FreeBlock*>(ptrForIndex(arena, order, index, minBlockSize));
        block->next = arena.freeLists[order.value];
        arena.freeLists[order.value] = block;
        setBlockFree(arena, order, index, true);
    }

    FreeBlock* AlignedBuddyAllocator::removeFreeBlock(
        ArenaState& arena,
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const Bytes minBlockSize
    ) {
        FreeBlock* previous = nullptr;
        auto* current = arena.freeLists[order.value];
        void* expected = ptrForIndex(arena, order, index, minBlockSize);
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

    AlignedContinuousMemoryBlock AlignedBuddyAllocator::allocateFromExistingArena(
        const Bytes size,
        const BuddyOrder targetOrder,
        const Bytes minBlockSize,
        const BuddyOrder maxOrder
    ) {
        ArenaState* selectedArena = nullptr;
        BuddyOrder selectedOrder = targetOrder;
        for (auto* arena = arenas; arena != nullptr && selectedArena == nullptr; arena = arena->next) {
            for (BuddyOrder order = targetOrder; order <= maxOrder; ++order.value) {
                if (arena->freeLists[order.value] != nullptr) {
                    selectedArena = arena;
                    selectedOrder = order;
                    break;
                }
            }
        }

        if (selectedArena == nullptr) {
            return emptyBlock(alignment);
        }

        auto* block = selectedArena->freeLists[selectedOrder.value];
        const BuddyBlockIndex selectedIndex = blockIndex(*selectedArena, block, selectedOrder, minBlockSize);
        if (removeFreeBlock(*selectedArena, selectedOrder, selectedIndex, minBlockSize) == nullptr) {
            throw std::runtime_error("AlignedBuddyAllocator: selected free block disappeared");
        }

        while (selectedOrder > targetOrder) {
            --selectedOrder.value;
            const BuddyBlockIndex leftIndex = blockIndex(*selectedArena, block, selectedOrder, minBlockSize);
            const BuddyBlockIndex rightIndex{leftIndex.value ^ 1};
            pushFreeBlock(*selectedArena, selectedOrder, rightIndex, minBlockSize);
        }

        return AlignedContinuousMemoryBlock{block, bytesForOrder(targetOrder, minBlockSize), alignment};
    }

    void AlignedBuddyAllocator::deallocate(
        const AlignedContinuousMemoryBlock block,
        BuddyOrder order,
        const Bytes minBlockSize,
        const BuddyOrder maxOrder
    ) {
        auto* arena = findArenaContaining(block.ptr, bytesForOrder(maxOrder, minBlockSize));
        if (arena == nullptr) {
            throw std::invalid_argument("AlignedBuddyAllocator: block does not belong to this allocator");
        }

        BuddyBlockIndex index = blockIndex(*arena, block.ptr, order, minBlockSize);
        while (order < maxOrder) {
            const BuddyBlockIndex buddyIndex{index.value ^ 1};
            if (!isBlockFree(*arena, order, buddyIndex)) {
                break;
            }
            if (removeFreeBlock(*arena, order, buddyIndex, minBlockSize) == nullptr) {
                throw std::runtime_error("AlignedBuddyAllocator: bitmap and free list are out of sync");
            }
            index.value /= 2;
            ++order.value;
        }
        pushFreeBlock(*arena, order, index, minBlockSize);
    }
}
