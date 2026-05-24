#include "ResourceManagement/Memory/BuddyAlloc/AlignedBuddyAllocator.h"

#include "ResourceManagement/Memory/BuddyAlloc/BuddyOrderThreshold.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {
    AlignedBuddyAllocator::AlignedBuddyAllocator(
        const Alignment alignment,
        BumpAlloc::BumpAllocator& bumpAllocator
    ) : alignment(alignment),
        arenaStates(nullptr),
        next(nullptr) {
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
        const BuddyOrder targetOrder = orderFor(size);
        if (targetOrder > arenaStates->maxOrder) {
            return AlignedContinuousMemoryBlock::empty(alignment);
        }
        return allocateFromExistingArena(targetOrder);
    }

    AlignedContinuousMemoryBlock AlignedBuddyAllocator::allocateWithNewArena(
        const Bytes size,
        BumpAlloc::BumpAllocator& bumpAllocator
    ) {
        const BuddyOrder targetOrder = orderFor(size);
        (void)createArena(bumpAllocator);
        return allocateFromExistingArena(targetOrder);
    }

    BuddyOrder AlignedBuddyAllocator::orderFor(const Bytes size) const {
        const Bytes minBlockSize = arenaStates->minBlockSize;
        const BuddyOrder maxOrder = arenaStates->maxOrder;
        const Bytes rounded = Bytes::max(size, minBlockSize).roundUpToPowerOfTwo();
        if (rounded > maxOrder.bytesFor(minBlockSize)) {
            throw std::invalid_argument("AlignedBuddyAllocator: requested size exceeds arena size");
        }
        BuddyOrder order{0};
        Bytes current = minBlockSize;
        while (current < rounded) {
            current = current * 2;
            order = order.next();
        }
        return order;
    }

    ArenaState* AlignedBuddyAllocator::createArena(BumpAlloc::BumpAllocator& bumpAllocator) {
        auto* arena = bumpAllocator.allocateArena(alignment);
        auto* state = new ArenaState(arena);
        state->setNext(arenaStates);
        arenaStates = state;
        return state;
    }

    ArenaState* AlignedBuddyAllocator::findArenaContaining(void* ptr) const {
        auto* current = arenaStates;
        while (current != nullptr) {
            if (current->arena->block.contains(ptr)) {
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
            if (arenaState->findAvailableOrder(targetOrder, selectedOrder)) {
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
            return AlignedContinuousMemoryBlock::empty(alignment);
        }

        auto* block = selectedArenaState->useBlockWithSplittingFreeList(selectedOrder, targetOrder);

        return AlignedContinuousMemoryBlock{block, targetOrder.bytesFor(selectedArenaState->minBlockSize), alignment};
    }

    void AlignedBuddyAllocator::deallocate(const AlignedContinuousMemoryBlock block) {
        BuddyOrder order = orderFor(block.size);
        auto* arenaState = findArenaContaining(block.ptr);
        if (arenaState == nullptr) {
            throw std::invalid_argument("AlignedBuddyAllocator: block does not belong to this allocator");
        }

        arenaState->unuseBlockWithMergingFreeList(block.ptr, order);
    }
}
