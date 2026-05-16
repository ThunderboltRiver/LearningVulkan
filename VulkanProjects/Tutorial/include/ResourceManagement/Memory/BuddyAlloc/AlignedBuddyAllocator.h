#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ALIGNEDBUDDYALLOCATOR_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ALIGNEDBUDDYALLOCATOR_H

#include "ResourceManagement/Memory/Alignment.h"
#include "ResourceManagement/Memory/BuddyAlloc/ArenaState.h"
#include "ResourceManagement/Memory/BuddyAlloc/BuddyOrder.h"
#include "ResourceManagement/Memory/BumpAlloc/BumpAllocator.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * 同じAlignmentを要求するアリーナ群。
     * アライメント別に分けることで、返却時に block.alignment から所属先を特定できる。
     */
    struct AlignedBuddyAllocator {
        Alignment alignment;
        ArenaState* arenaStates;
        AlignedBuddyAllocator* next;

        AlignedBuddyAllocator(Alignment alignment, BumpAlloc::BumpAllocator& bumpAllocator);

        ~AlignedBuddyAllocator();

        void setNext(AlignedBuddyAllocator* nextAllocator);

        [[nodiscard]] bool satisfies(Alignment requestedAlignment) const;

        [[nodiscard]] AlignedContinuousMemoryBlock tryAllocate(Bytes size);

        [[nodiscard]] AlignedContinuousMemoryBlock allocateWithNewArena(Bytes size, BumpAlloc::BumpAllocator& bumpAllocator);

        void deallocate(AlignedContinuousMemoryBlock block);

    private:
        [[nodiscard]] static Bytes calculateMinBlockSize(Bytes arenaSize);

        [[nodiscard]] static BuddyOrder calculateMaxOrder(Bytes arenaSize, Bytes minBlockSize);

        [[nodiscard]] BuddyOrder orderFor(Bytes size) const;

        [[nodiscard]] ArenaState* createArena(BumpAlloc::BumpAllocator& bumpAllocator, Bytes minBlockSize, BuddyOrder maxOrder);

        [[nodiscard]] ArenaState* findArenaContaining(void* ptr) const;

        /**
         * 要求order以上のorderに対応するフリーリスト内にブロックがあるアリーナとそのオーダーを取得する。
         * @param targetOrder 要求order
         * @param selectedOrder 結果返却用. 要求order以上で、実際に空きが見つかったorder。見つからなかった場合の値は不定。
         * @return targetOrder以上のorderで空きが見つかったアリーナ状態。見つからなかった場合はnullptr。
         */
        [[nodiscard]] ArenaState* findArenaStateWithAvailableOrder(BuddyOrder targetOrder, BuddyOrder& selectedOrder) const;

        [[nodiscard]] AlignedContinuousMemoryBlock allocateFromExistingArena(BuddyOrder targetOrder);
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ALIGNEDBUDDYALLOCATOR_H
