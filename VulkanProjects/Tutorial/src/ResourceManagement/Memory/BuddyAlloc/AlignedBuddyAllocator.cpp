#include "ResourceManagement/Memory/BuddyAlloc/AlignedBuddyAllocator.h"

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
        const BuddyOrder maxOrder,
        const Bytes minBlockSize,
        BumpAlloc::BumpAllocator& bumpAllocator
    ) : alignment(alignment), arenaStates(nullptr), next(nullptr) {
        (void)createArena(maxOrder, minBlockSize, bumpAllocator);
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

    AlignedContinuousMemoryBlock AlignedBuddyAllocator::allocateFromExistingArena(
        const Bytes size,
        const BuddyOrder targetOrder,
        const Bytes minBlockSize,
        const BuddyOrder maxOrder
    ) {
        ArenaState* selectedArenaState = nullptr;
        BuddyOrder selectedOrder = targetOrder;
        for (auto* arenaState = arenaStates; arenaState != nullptr && selectedArenaState == nullptr; arenaState = arenaState->next) {
            if (arenaState->findAvailableOrder(targetOrder, maxOrder, selectedOrder)) {
                selectedArenaState = arenaState;
            }
        }

        if (selectedArenaState == nullptr) {
            return emptyBlock(alignment);
        }

        auto* block = selectedArenaState->freeLists[selectedOrder.value];
        const BuddyBlockIndex selectedIndex = selectedArenaState->blockIndex(block, selectedOrder, minBlockSize);
        if (selectedArenaState->removeFreeBlock(selectedOrder, selectedIndex, minBlockSize) == nullptr) {
            throw std::runtime_error("AlignedBuddyAllocator: selected free block disappeared");
        }

        while (selectedOrder > targetOrder) {
            --selectedOrder.value;
            const BuddyBlockIndex leftIndex = selectedArenaState->blockIndex(block, selectedOrder, minBlockSize);
            const BuddyBlockIndex rightIndex{leftIndex.value ^ 1};
            selectedArenaState->pushFreeBlock(selectedOrder, rightIndex, minBlockSize);
        }

        return AlignedContinuousMemoryBlock{block, targetOrder.bytesFor(minBlockSize), alignment};
    }

    void AlignedBuddyAllocator::deallocate(
        const AlignedContinuousMemoryBlock block,
        BuddyOrder order,
        const Bytes minBlockSize,
        const BuddyOrder maxOrder
    ) {
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
