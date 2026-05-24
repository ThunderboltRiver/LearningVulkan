#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ORDERFREEBLOCKS_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ORDERFREEBLOCKS_H

#include "ResourceManagement/Memory/BuddyAlloc/BuddyBlockIndex.h"
#include "ResourceManagement/Memory/BuddyAlloc/BuddyFreeBitmap.h"
#include "ResourceManagement/Memory/BuddyAlloc/FreeBlock.h"

#include <cstddef>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * 1つのorderに属するfree block集合。
     * free list と bitmap は同じ空き状態を表すため、この型の中で同期して更新する。
     */
    struct OrderFreeBlocks {
    private:
        /** order内の空きブロックリスト先頭。ブロック本体の先頭をFreeBlockとして使う。 */
        FreeBlock* freeList;

        /** freeList上に存在するブロックindexを高速に判定するためのbitmap。 */
        BuddyFreeBitmap freeBitmap;

    public:
        /** 配列要素として後から初期化できる空状態。 */
        OrderFreeBlocks();

        /** blockCount 個のブロック状態を追跡できるbitmapを事前作成する。 */
        explicit OrderFreeBlocks(std::size_t blockCount);

        OrderFreeBlocks(const OrderFreeBlocks&) = delete;
        OrderFreeBlocks& operator=(const OrderFreeBlocks&) = delete;

        OrderFreeBlocks(OrderFreeBlocks&& other) noexcept;
        OrderFreeBlocks& operator=(OrderFreeBlocks&& other) noexcept;

        /** free list に1つ以上の空きブロックがあるかを返す。 */
        [[nodiscard]] bool hasFreeBlock() const;

        /** free list の先頭ブロックを返す。存在しない場合はnullptr。 */
        [[nodiscard]] FreeBlock* firstFreeBlock() const;

        /** index のブロックがfree list上に存在するかを返す。 */
        [[nodiscard]] bool isFree(BuddyBlockIndex index) const;

        /** block をfree list先頭へ追加し、対応するbitmap bitを立てる。 */
        void add(FreeBlock* block, BuddyBlockIndex index);

        /** block をfree listから外し、対応するbitmap bitを落とす。freeでなければnullptrを返す。 */
        [[nodiscard]] FreeBlock* remove(FreeBlock* block, BuddyBlockIndex index);
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ORDERFREEBLOCKS_H
