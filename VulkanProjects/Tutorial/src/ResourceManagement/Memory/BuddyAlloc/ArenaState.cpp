#include "ResourceManagement/Memory/BuddyAlloc/ArenaState.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {
    ArenaState::ArenaState(
        BumpAlloc::AlignedArena* arena,
        const BuddyOrder maxOrder,
        const Bytes minBlockSize
    ) : arena(arena),
        freeLists{},
        freeBitmaps{},
        minBlockSize(minBlockSize),
        maxOrder(maxOrder),
        next(nullptr) {
        // orderごとのフリーリスト・フリービットマップの初期化
        for (BuddyOrder order{0}; order <= maxOrder; order = order.next()) {
            freeLists[order.value()] = nullptr;
            const std::size_t blockCount = arena->block.size.value() / order.bytesFor(minBlockSize).value();
            freeBitmaps[order.value()] = BuddyFreeBitmap(blockCount);
        }

        auto* block = static_cast<FreeBlock*>(arena->block.ptr);
        block->previous = nullptr;
        block->next = nullptr;

        // 最初はmaxOrderのフリーリストにアリーナ全体が空きブロックとして存在する状態にする
        freeLists[maxOrder.value()] = block;
        freeBitmaps[maxOrder.value()].setFree(BuddyBlockIndex{0}, true);
    }

    void ArenaState::setNext(ArenaState* nextArenaState) {
        next = nextArenaState;
    }

    bool ArenaState::findAvailableOrder(
        const BuddyOrder targetOrder,
        BuddyOrder& selectedOrder
    ) const {
        for (BuddyOrder order = targetOrder; order <= maxOrder; order = order.next()) {
            if (freeLists[order.value()] != nullptr) {
                selectedOrder = order;
                return true;
            }
        }
        return false;
    }

    BuddyBlockIndex ArenaState::blockIndex(
        const void* ptr,
        const BuddyOrder order
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
        const BuddyBlockIndex index
    ) const {
        const auto base = reinterpret_cast<std::uintptr_t>(arena->block.ptr);
        return reinterpret_cast<void*>(base + order.bytesFor(minBlockSize).value() * index.value());
    }

    bool ArenaState::isBlockFree(const BuddyOrder order, const BuddyBlockIndex index) const {
        return freeBitmaps[order.value()].isFree(index);
    }

    void ArenaState::setBlockFree(
        const BuddyOrder order,
        const BuddyBlockIndex index,
        const bool value
    ) {
        freeBitmaps[order.value()].setFree(index, value);
    }

    void ArenaState::addFreeBlock(
        const BuddyOrder order,
        const BuddyBlockIndex index
    ) {
        auto* block = static_cast<FreeBlock*>(ptrForIndex(order, index));
        block->previous = nullptr;
        block->next = freeLists[order.value()];
        if (freeLists[order.value()] != nullptr) {
            freeLists[order.value()]->previous = block;
        }
        freeLists[order.value()] = block;
        setBlockFree(order, index, true);
    }

    FreeBlock* ArenaState::removeFreeBlock(
        const BuddyOrder order,
        const BuddyBlockIndex index
    ) {
        if (!isBlockFree(order, index)) {
            return nullptr;
        }

        auto* block = static_cast<FreeBlock*>(ptrForIndex(order, index));
        // 前後のブロックを繋ぎ直す
        if (block->previous == nullptr) {
            freeLists[order.value()] = block->next;
        } else {
            block->previous->next = block->next;
        }
        if (block->next != nullptr) {
            block->next->previous = block->previous;
        }

        // bitmapを更新して、ブロックをfreeリストから切り離す
        setBlockFree(order, index, false);
        block->previous = nullptr;
        block->next = nullptr;
        return block;
    }

    FreeBlock* ArenaState::removeFreeBlock(
        const BuddyOrder order
    ) {
        auto* block = freeLists[order.value()];
        const BuddyBlockIndex selectedIndex = blockIndex(block, order);
        if (removeFreeBlock(order, selectedIndex) == nullptr) {
            throw std::runtime_error("ArenaState: selected free block disappeared");
        }
        return block;
    }

    void* ArenaState::useBlockWithSplittingFreeList(
        const BuddyOrder selectedOrder,
        const BuddyOrder targetOrder
    ) {
        // 一つのブロックをフリーリストから切り離す
        auto* result = removeFreeBlock(selectedOrder);

        // 要求orderに達するまで、切り離したblockを半分ずつ分割して、分割された片方をフリーリストへ追加する
        BuddyOrder currentOrder = selectedOrder;
        while (currentOrder > targetOrder) {
            // ブロックを半分に分割
            currentOrder = currentOrder.previous();

            // 分割された片方のブロック（resultのアドレスを持たない方）をフリーリストに追加する
            const BuddyBlockIndex leftIndex = blockIndex(result, currentOrder);
            addFreeBlock(currentOrder, leftIndex.buddy());
        }

        return result;
    }

    void ArenaState::unuseBlockWithMergingFreeList(
        void* ptr,
        BuddyOrder order
    ) {
        BuddyBlockIndex index = blockIndex(ptr, order);
        // ブロックの相方がfreeリスト上に存在する限り統合し続ける
        while (order < maxOrder) {
            const BuddyBlockIndex buddyIndex = index.buddy();
            // 相方がfreeじゃないなら統合ループを抜ける
            if (!isBlockFree(order, buddyIndex)) {
                break;
            }
            // 相方がfeeならブロックの統合を行うため、フリーリストから相方を除去する
            if (removeFreeBlock(order, buddyIndex) == nullptr) {
                throw std::runtime_error("ArenaState: bitmap and free list are out of sync");
            }
            // indexを親indexへ更新して、次のループでさらに上位orderのbuddyと統合できるか確認する
            index = index.parent();
            order = order.next();
        }
        // 統合後のブロックをfreeリストへ追加する
        addFreeBlock(order, index);
    }
}
