#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ARENASTATE_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ARENASTATE_H

#include "ResourceManagement/Memory/Alignment.h"
#include "ResourceManagement/Memory/BuddyAlloc/BuddyFreeBitmap.h"
#include "ResourceManagement/Memory/BuddyAlloc/BuddyOrder.h"
#include "ResourceManagement/Memory/BuddyAlloc/BuddyOrderThreshold.h"
#include "ResourceManagement/Memory/BuddyAlloc/FreeBlock.h"
#include "ResourceManagement/Memory/BumpAlloc/AlignedArena.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * 1つのアリーナに紐づくBuddyAllocator用メタデータ。
     * freeLists と freeBitmaps は同じ空き状態を表し、更新は helper 経由で同期する。
     */
    struct ArenaState {
        /** このメタデータが管理する実アリーナ。 */
        BumpAlloc::AlignedArena* arena;

        /** orderごとの空きブロックリスト。ブロック本体の先頭をFreeBlockとして使う。 */
        FreeBlock* freeLists[BUDDY_ORDER_COUNT];

        /** orderごとの空き状態bitmap。buddy統合時に相方ブロックが空きかを高速に確認する。 */
        BuddyFreeBitmap freeBitmaps[BUDDY_ORDER_COUNT];

        /** このアリーナで扱う最小ブロックサイズ。order 0 のサイズに相当する。 */
        Bytes minBlockSize;

        /** このアリーナで扱う最大order。arena全体を表すorderに相当する。 */
        BuddyOrder maxOrder;

        /** 同じAlignmentを持つ次のアリーナメタデータ。 */
        ArenaState* next;

        explicit ArenaState(BumpAlloc::AlignedArena* arena);

        void setNext(ArenaState* nextArenaState);

        [[nodiscard]] bool findAvailableOrder(BuddyOrder targetOrder, BuddyOrder& selectedOrder) const;

        [[nodiscard]] void *useBlockWithSplittingFreeList(BuddyOrder selectedOrder, BuddyOrder targetOrder);

        void unuseBlockWithMergingFreeList(void* ptr, BuddyOrder order);

    private:
        [[nodiscard]] static Bytes calculateMinBlockSize(Bytes arenaSize, Alignment alignment);

        [[nodiscard]] static BuddyOrder calculateMaxOrder(Bytes arenaSize, Bytes minBlockSize);

        void initializeFreeBlockTracking();

        [[nodiscard]] BuddyBlockIndex blockIndex(const void* ptr, BuddyOrder order) const;

        [[nodiscard]] void* ptrForIndex(BuddyOrder order, BuddyBlockIndex index) const;

        void addFreeBlock(BuddyOrder order, BuddyBlockIndex index);

        [[nodiscard]] FreeBlock* removeFreeBlock(BuddyOrder order, BuddyBlockIndex index);

        [[nodiscard]] FreeBlock* removeFreeBlock(BuddyOrder order);
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ARENASTATE_H
