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
        ArenaState* arenas;
        AlignedBuddyAllocator* next;

        AlignedBuddyAllocator(Alignment alignment, AlignedBuddyAllocator* next);

        [[nodiscard]] bool satisfies(Alignment requestedAlignment) const;

        [[nodiscard]] AlignedContinuousMemoryBlock tryAllocate(Bytes size, BuddyOrder targetOrder, Bytes minBlockSize, BuddyOrder maxOrder);

        [[nodiscard]] AlignedContinuousMemoryBlock allocateWithNewArena(
            Bytes size,
            BuddyOrder targetOrder,
            Bytes minBlockSize,
            BuddyOrder maxOrder,
            BumpAlloc::BumpAllocator& bumpAllocator
        );

        void deallocate(AlignedContinuousMemoryBlock block, BuddyOrder order, Bytes minBlockSize, BuddyOrder maxOrder);

    private:
        [[nodiscard]] ArenaState* createArena(BuddyOrder maxOrder, Bytes minBlockSize, BumpAlloc::BumpAllocator& bumpAllocator);

        [[nodiscard]] static Bytes bytesForOrder(BuddyOrder order, Bytes minBlockSize);

        [[nodiscard]] ArenaState* findArenaContaining(void* ptr, Bytes arenaSize) const;

        [[nodiscard]] static BuddyBlockIndex blockIndex(const ArenaState& arena, const void* ptr, BuddyOrder order, Bytes minBlockSize);

        [[nodiscard]] static void* ptrForIndex(const ArenaState& arena, BuddyOrder order, BuddyBlockIndex index, Bytes minBlockSize);

        [[nodiscard]] static bool isBlockFree(const ArenaState& arena, BuddyOrder order, BuddyBlockIndex index);

        static void setBlockFree(ArenaState& arena, BuddyOrder order, BuddyBlockIndex index, bool value);

        static void pushFreeBlock(ArenaState& arena, BuddyOrder order, BuddyBlockIndex index, Bytes minBlockSize);

        [[nodiscard]] static FreeBlock* removeFreeBlock(ArenaState& arena, BuddyOrder order, BuddyBlockIndex index, Bytes minBlockSize);

        [[nodiscard]] AlignedContinuousMemoryBlock allocateFromExistingArena(Bytes size, BuddyOrder targetOrder, Bytes minBlockSize, BuddyOrder maxOrder);
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ALIGNEDBUDDYALLOCATOR_H
