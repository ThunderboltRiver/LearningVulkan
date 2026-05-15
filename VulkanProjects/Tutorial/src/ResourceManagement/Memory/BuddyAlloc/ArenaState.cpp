#include "ResourceManagement/Memory/BuddyAlloc/ArenaState.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {
    ArenaState::ArenaState(
        BumpAlloc::AlignedArena* arena,
        const BuddyOrder maxOrder,
        const Bytes minBlockSize
    ) : arena(arena), freeLists{}, freeBitmaps{}, next(nullptr) {
        for (BuddyOrder order{0}; order <= maxOrder; ++order.value) {
            freeLists[order.value] = nullptr;
            const std::size_t blockCount = arena->block.size.value() / order.bytesFor(minBlockSize).value();
            freeBitmaps[order.value] = BuddyFreeBitmap(blockCount);
        }

        auto* block = static_cast<FreeBlock*>(arena->block.ptr);
        block->next = nullptr;
        freeLists[maxOrder.value] = block;
        freeBitmaps[maxOrder.value].setFree(BuddyBlockIndex{0}, true);
    }

    void ArenaState::setNext(ArenaState* nextArenaState) {
        next = nextArenaState;
    }

    bool ArenaState::findAvailableOrder(
        const BuddyOrder targetOrder,
        const BuddyOrder maxOrder,
        BuddyOrder& selectedOrder
    ) const {
        for (BuddyOrder order = targetOrder; order <= maxOrder; ++order.value) {
            if (freeLists[order.value] != nullptr) {
                selectedOrder = order;
                return true;
            }
        }
        return false;
    }

    BuddyBlockIndex ArenaState::blockIndex(
        const void* ptr,
        const BuddyOrder order,
        const Bytes minBlockSize
    ) const {
        const auto address = reinterpret_cast<std::uintptr_t>(ptr);
        const auto base = reinterpret_cast<std::uintptr_t>(arena->block.ptr);
        const auto offset = address - base;
        const auto blockSize = order.bytesFor(minBlockSize).value();
        if (offset % blockSize != 0) {
            throw std::invalid_argument("ArenaState: pointer is not aligned to block order");
        }
        return BuddyBlockIndex{offset / blockSize};
    }

    void* ArenaState::ptrForIndex(
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const Bytes minBlockSize
    ) const {
        const auto base = reinterpret_cast<std::uintptr_t>(arena->block.ptr);
        return reinterpret_cast<void*>(base + order.bytesFor(minBlockSize).value() * index.value);
    }

    bool ArenaState::isBlockFree(const BuddyOrder order, const BuddyBlockIndex index) const {
        return freeBitmaps[order.value].isFree(index);
    }

    void ArenaState::setBlockFree(
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const bool value
    ) {
        freeBitmaps[order.value].setFree(index, value);
    }

    void ArenaState::pushFreeBlock(
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const Bytes minBlockSize
    ) {
        auto* block = static_cast<FreeBlock*>(ptrForIndex(order, index, minBlockSize));
        block->next = freeLists[order.value];
        freeLists[order.value] = block;
        setBlockFree(order, index, true);
    }

    FreeBlock* ArenaState::removeFreeBlock(
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const Bytes minBlockSize
    ) {
        FreeBlock* previous = nullptr;
        auto* current = freeLists[order.value];
        void* expected = ptrForIndex(order, index, minBlockSize);
        while (current != nullptr) {
            if (current == expected) {
                if (previous == nullptr) {
                    freeLists[order.value] = current->next;
                } else {
                    previous->next = current->next;
                }
                setBlockFree(order, index, false);
                current->next = nullptr;
                return current;
            }
            previous = current;
            current = current->next;
        }
        return nullptr;
    }
}
