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

    Bytes AlignedBuddyAllocator::bytesForOrder(const BuddyOrder order, const Bytes minBlockSize) {
        Bytes result = minBlockSize;
        for (std::size_t i = 0; i < order.value; ++i) {
            result = result * 2;
        }
        return result;
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

    BuddyBlockIndex AlignedBuddyAllocator::blockIndex(
        const ArenaState& arenaState,
        const void* ptr,
        const BuddyOrder order,
        const Bytes minBlockSize
    ) {
        const auto address = reinterpret_cast<std::uintptr_t>(ptr);
        const auto base = reinterpret_cast<std::uintptr_t>(arenaState.arena->block.ptr);
        const auto offset = address - base;
        const auto blockSize = bytesForOrder(order, minBlockSize).value();
        if (offset % blockSize != 0) {
            throw std::invalid_argument("AlignedBuddyAllocator: pointer is not aligned to block order");
        }
        return BuddyBlockIndex{offset / blockSize};
    }

    void* AlignedBuddyAllocator::ptrForIndex(
        const ArenaState& arenaState,
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const Bytes minBlockSize
    ) {
        const auto base = reinterpret_cast<std::uintptr_t>(arenaState.arena->block.ptr);
        return reinterpret_cast<void*>(base + bytesForOrder(order, minBlockSize).value() * index.value);
    }

    void AlignedBuddyAllocator::setBlockFree(
        ArenaState& arenaState,
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const bool value
    ) {
        arenaState.freeBitmaps[order.value].setFree(index, value);
    }

    bool AlignedBuddyAllocator::isBlockFree(
        const ArenaState& arenaState,
        const BuddyOrder order,
        const BuddyBlockIndex index
    ) {
        return arenaState.freeBitmaps[order.value].isFree(index);
    }

    void AlignedBuddyAllocator::pushFreeBlock(
        ArenaState& arenaState,
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const Bytes minBlockSize
    ) {
        auto* block = static_cast<FreeBlock*>(ptrForIndex(arenaState, order, index, minBlockSize));
        block->next = arenaState.freeLists[order.value];
        arenaState.freeLists[order.value] = block;
        setBlockFree(arenaState, order, index, true);
    }

    FreeBlock* AlignedBuddyAllocator::removeFreeBlock(
        ArenaState& arenaState,
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const Bytes minBlockSize
    ) {
        FreeBlock* previous = nullptr;
        auto* current = arenaState.freeLists[order.value];
        void* expected = ptrForIndex(arenaState, order, index, minBlockSize);
        while (current != nullptr) {
            if (current == expected) {
                if (previous == nullptr) {
                    arenaState.freeLists[order.value] = current->next;
                } else {
                    previous->next = current->next;
                }
                setBlockFree(arenaState, order, index, false);
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
        ArenaState* selectedArenaState = nullptr;
        BuddyOrder selectedOrder = targetOrder;
        for (auto* arenaState = arenaStates; arenaState != nullptr && selectedArenaState == nullptr; arenaState = arenaState->next) {
            for (BuddyOrder order = targetOrder; order <= maxOrder; ++order.value) {
                if (arenaState->freeLists[order.value] != nullptr) {
                    selectedArenaState = arenaState;
                    selectedOrder = order;
                    break;
                }
            }
        }

        if (selectedArenaState == nullptr) {
            return emptyBlock(alignment);
        }

        auto* block = selectedArenaState->freeLists[selectedOrder.value];
        const BuddyBlockIndex selectedIndex = blockIndex(*selectedArenaState, block, selectedOrder, minBlockSize);
        if (removeFreeBlock(*selectedArenaState, selectedOrder, selectedIndex, minBlockSize) == nullptr) {
            throw std::runtime_error("AlignedBuddyAllocator: selected free block disappeared");
        }

        while (selectedOrder > targetOrder) {
            --selectedOrder.value;
            const BuddyBlockIndex leftIndex = blockIndex(*selectedArenaState, block, selectedOrder, minBlockSize);
            const BuddyBlockIndex rightIndex{leftIndex.value ^ 1};
            pushFreeBlock(*selectedArenaState, selectedOrder, rightIndex, minBlockSize);
        }

        return AlignedContinuousMemoryBlock{block, bytesForOrder(targetOrder, minBlockSize), alignment};
    }

    void AlignedBuddyAllocator::deallocate(
        const AlignedContinuousMemoryBlock block,
        BuddyOrder order,
        const Bytes minBlockSize,
        const BuddyOrder maxOrder
    ) {
        auto* arenaState = findArenaContaining(block.ptr, bytesForOrder(maxOrder, minBlockSize));
        if (arenaState == nullptr) {
            throw std::invalid_argument("AlignedBuddyAllocator: block does not belong to this allocator");
        }

        BuddyBlockIndex index = blockIndex(*arenaState, block.ptr, order, minBlockSize);
        while (order < maxOrder) {
            const BuddyBlockIndex buddyIndex{index.value ^ 1};
            if (!isBlockFree(*arenaState, order, buddyIndex)) {
                break;
            }
            if (removeFreeBlock(*arenaState, order, buddyIndex, minBlockSize) == nullptr) {
                throw std::runtime_error("AlignedBuddyAllocator: bitmap and free list are out of sync");
            }
            index.value /= 2;
            ++order.value;
        }
        pushFreeBlock(*arenaState, order, index, minBlockSize);
    }
}
