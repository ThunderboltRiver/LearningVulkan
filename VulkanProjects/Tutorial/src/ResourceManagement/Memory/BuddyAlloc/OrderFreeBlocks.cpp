#include "ResourceManagement/Memory/BuddyAlloc/OrderFreeBlocks.h"

#include <utility>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    OrderFreeBlocks::OrderFreeBlocks()
        : freeList(nullptr), freeBitmap() {
    }

    OrderFreeBlocks::OrderFreeBlocks(const std::size_t blockCount)
        : freeList(nullptr), freeBitmap(blockCount) {
    }

    OrderFreeBlocks::OrderFreeBlocks(OrderFreeBlocks&& other) noexcept
        : freeList(other.freeList), freeBitmap(std::move(other.freeBitmap)) {
        other.freeList = nullptr;
    }

    OrderFreeBlocks& OrderFreeBlocks::operator=(OrderFreeBlocks&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        freeList = other.freeList;
        freeBitmap = std::move(other.freeBitmap);
        other.freeList = nullptr;
        return *this;
    }

    bool OrderFreeBlocks::hasFreeBlock() const {
        return freeList != nullptr;
    }

    FreeBlock* OrderFreeBlocks::firstFreeBlock() const {
        return freeList;
    }

    bool OrderFreeBlocks::isFree(const BuddyBlockIndex index) const {
        return freeBitmap.isFree(index);
    }

    void OrderFreeBlocks::add(FreeBlock* block, const BuddyBlockIndex index) {
        block->previous = nullptr;
        block->next = freeList;
        if (freeList != nullptr) {
            freeList->previous = block;
        }
        freeList = block;
        freeBitmap.setFree(index, true);
    }

    FreeBlock* OrderFreeBlocks::remove(FreeBlock* block, const BuddyBlockIndex index) {
        if (!freeBitmap.isFree(index)) {
            return nullptr;
        }

        if (block->previous == nullptr) {
            freeList = block->next;
        } else {
            block->previous->next = block->next;
        }
        if (block->next != nullptr) {
            block->next->previous = block->previous;
        }

        freeBitmap.setFree(index, false);
        block->previous = nullptr;
        block->next = nullptr;
        return block;
    }
}
