#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ARENASTATE_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ARENASTATE_H

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

        /** 同じAlignmentを持つ次のアリーナメタデータ。 */
        ArenaState* next;

        ArenaState(BumpAlloc::AlignedArena* arena, BuddyOrder maxOrder, Bytes minBlockSize);

        void setNext(ArenaState* nextArenaState);

        [[nodiscard]] bool findAvailableOrder(BuddyOrder targetOrder, BuddyOrder maxOrder, BuddyOrder& selectedOrder) const;

        [[nodiscard]] BuddyBlockIndex blockIndex(const void* ptr, BuddyOrder order, Bytes minBlockSize) const;

        [[nodiscard]] void* ptrForIndex(BuddyOrder order, BuddyBlockIndex index, Bytes minBlockSize) const;

        [[nodiscard]] bool isBlockFree(BuddyOrder order, BuddyBlockIndex index) const;

        void setBlockFree(BuddyOrder order, BuddyBlockIndex index, bool value);

        void pushFreeBlock(BuddyOrder order, BuddyBlockIndex index, Bytes minBlockSize);

        [[nodiscard]] FreeBlock* removeFreeBlock(BuddyOrder order, BuddyBlockIndex index, Bytes minBlockSize);
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ARENASTATE_H
