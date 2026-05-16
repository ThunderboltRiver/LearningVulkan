#include "ResourceManagement/Memory/BuddyAlloc/AlignedBuddyAllocator.h"

#include "ResourceManagement/Memory/BuddyAlloc/BuddyOrderThreshold.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {
    namespace {
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

    AlignedBuddyAllocator::AlignedBuddyAllocator(
        const Alignment alignment,
        BumpAlloc::BumpAllocator& bumpAllocator
    ) : alignment(alignment),
        arenaStates(nullptr),
        next(nullptr),
        minBlockSize(calculateMinBlockSize(bumpAllocator.getArenaSize())),
        maxOrder(calculateMaxOrder(bumpAllocator.getArenaSize(), minBlockSize)) {
        (void)createArena(bumpAllocator);
    }

    AlignedBuddyAllocator::~AlignedBuddyAllocator() {
        auto* arenaState = arenaStates;
        while (arenaState != nullptr) {
            auto* nextArenaState = arenaState->next;
            delete arenaState;
            arenaState = nextArenaState;
        }
        arenaStates = nullptr;
    }

    void AlignedBuddyAllocator::setNext(AlignedBuddyAllocator* nextAllocator) {
        next = nextAllocator;
    }

    bool AlignedBuddyAllocator::satisfies(const Alignment requestedAlignment) const {
        return alignment.bytes().value() % requestedAlignment.bytes().value() == 0;
    }

    AlignedContinuousMemoryBlock AlignedBuddyAllocator::tryAllocate(const Bytes size) {
        const BuddyOrder targetOrder = orderFor(size.max(alignment.bytes()));
        if (targetOrder > maxOrder) {
            return emptyBlock(alignment);
        }
        return allocateFromExistingArena(targetOrder);
    }

    AlignedContinuousMemoryBlock AlignedBuddyAllocator::allocateWithNewArena(
        const Bytes size,
        BumpAlloc::BumpAllocator& bumpAllocator
    ) {
        const BuddyOrder targetOrder = orderFor(size.max(alignment.bytes()));
        (void)createArena(bumpAllocator);
        return allocateFromExistingArena(targetOrder);
    }

    Bytes AlignedBuddyAllocator::calculateMinBlockSize(const Bytes arenaSize) {
        const std::size_t arenaExponent = arenaSize.log2PowerOfTwo();
        const std::size_t thresholdMinExponent = arenaExponent >= BUDDY_ORDER_THRESHOLD
            ? arenaExponent - BUDDY_ORDER_THRESHOLD
            : 0;
        const std::size_t freeBlockExponent = Bytes::fromSizeT(sizeof(FreeBlock)).roundUpToPowerOfTwo().log2PowerOfTwo();
        const std::size_t minBlockExponent = thresholdMinExponent >= freeBlockExponent
            ? thresholdMinExponent
            : freeBlockExponent;
        return Bytes::fromPowerOfTwoExponent(minBlockExponent);
    }

    BuddyOrder AlignedBuddyAllocator::calculateMaxOrder(const Bytes arenaSize, const Bytes minBlockSize) {
        if (minBlockSize > arenaSize) {
            throw std::invalid_argument("AlignedBuddyAllocator: min block size must not exceed arena size");
        }
        const std::size_t arenaExponent = arenaSize.log2PowerOfTwo();
        const std::size_t minBlockExponent = minBlockSize.log2PowerOfTwo();
        const std::size_t maxOrderValue = arenaExponent - minBlockExponent;
        if (maxOrderValue > BUDDY_ORDER_THRESHOLD) {
            throw std::invalid_argument("AlignedBuddyAllocator: too many buddy orders");
        }
        return BuddyOrder{static_cast<std::uint8_t>(maxOrderValue)};
    }

    BuddyOrder AlignedBuddyAllocator::orderFor(const Bytes size) const {
        const Bytes rounded = size.max(minBlockSize).roundUpToPowerOfTwo();
        if (rounded > maxOrder.bytesFor(minBlockSize)) {
            throw std::invalid_argument("AlignedBuddyAllocator: requested size exceeds arena size");
        }
        BuddyOrder order{0};
        Bytes current = minBlockSize;
        while (current < rounded) {
            current = current * 2;
            ++order.value;
        }
        return order;
    }

    ArenaState* AlignedBuddyAllocator::createArena(BumpAlloc::BumpAllocator& bumpAllocator) {
        auto* arena = bumpAllocator.allocateArena(alignment);
        auto* state = new ArenaState(arena, maxOrder, minBlockSize);
        state->setNext(arenaStates);
        arenaStates = state;
        return state;
    }

    ArenaState* AlignedBuddyAllocator::findArenaContaining(void* ptr, const Bytes arenaSize) const {
        auto* current = arenaStates;
        while (current != nullptr) {
            if (pointerInRange(ptr, current->arena->block.ptr, arenaSize)) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    ArenaState* AlignedBuddyAllocator::findArenaStateWithAvailableOrder(
        const BuddyOrder targetOrder,
        BuddyOrder& selectedOrder
    ) const {
        for (auto* arenaState = arenaStates; arenaState != nullptr; arenaState = arenaState->next) {
            if (arenaState->findAvailableOrder(targetOrder, maxOrder, selectedOrder)) {
                return arenaState;
            }
        }
        return nullptr;
    }

    AlignedContinuousMemoryBlock AlignedBuddyAllocator::allocateFromExistingArena(
        const BuddyOrder targetOrder
    ) {
        BuddyOrder selectedOrder = targetOrder;
        ArenaState* selectedArenaState = findArenaStateWithAvailableOrder(targetOrder, selectedOrder);

        if (selectedArenaState == nullptr) {
            return emptyBlock(alignment);
        }

        // orderのフリーリストから使用するブロックを１つ取り出し、フリーリストの分割を行う
        // TODO:一つのメソッドにまとめ、関数名を変える
        auto* block = selectedArenaState->popSelectedFreeBlock(selectedOrder, minBlockSize);
        selectedArenaState->splitBlockUntilTargetOrder(block, selectedOrder, targetOrder, minBlockSize);

        return AlignedContinuousMemoryBlock{block, targetOrder.bytesFor(minBlockSize), alignment};
    }

    void AlignedBuddyAllocator::deallocate(const AlignedContinuousMemoryBlock block) {
        BuddyOrder order = orderFor(block.size.max(block.alignment.bytes()));
        auto* arenaState = findArenaContaining(block.ptr, maxOrder.bytesFor(minBlockSize));
        if (arenaState == nullptr) {
            throw std::invalid_argument("AlignedBuddyAllocator: block does not belong to this allocator");
        }

        BuddyBlockIndex index = arenaState->blockIndex(block.ptr, order, minBlockSize);
        while (order < maxOrder) {
            const BuddyBlockIndex buddyIndex{index.value ^ 1};
            if (!arenaState->isBlockFree(order, buddyIndex)) {
                break;
            }
            if (arenaState->removeFreeBlock(order, buddyIndex, minBlockSize) == nullptr) {
                throw std::runtime_error("AlignedBuddyAllocator: bitmap and free list are out of sync");
            }
            index.value /= 2;
            ++order.value;
        }
        arenaState->pushFreeBlock(order, index, minBlockSize);
    }
}
