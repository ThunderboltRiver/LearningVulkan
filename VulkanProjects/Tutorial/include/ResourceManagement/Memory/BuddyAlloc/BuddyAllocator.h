#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOCATOR_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOCATOR_H

#include "ResourceManagement/Memory/BuddyAlloc/AlignedBuddyAllocator.h"
#include "ResourceManagement/Memory/BumpAlloc/BumpAllocator.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * アリーナを2の冪乗サイズのブロックへ分割・統合して管理するアロケータ。
     * アライメントごとに独立したアリーナ群を持ち、free list と bitmap を同期して空き状態を管理する。
     */
    class BuddyAllocator {
        BumpAlloc::BumpAllocator _bumpAllocator;

        /** Alignmentごとに分かれたBuddyAllocator状態の侵入的リスト先頭。 */
        AlignedBuddyAllocator* _alignedAllocators;

        [[nodiscard]] AlignedBuddyAllocator* getOrCreateAlignedAllocator(Alignment alignment);

        [[nodiscard]] AlignedBuddyAllocator* findAlignedAllocator(Alignment alignment) const;

        void destroyMetadata() noexcept;

    public:
        explicit BuddyAllocator(Bytes arenaSize = DEFAULT_ARENA_SIZE);

        BuddyAllocator(const BuddyAllocator&) = delete;
        BuddyAllocator& operator=(const BuddyAllocator&) = delete;

        /** size以上かつalignmentを満たす最小orderのブロックを確保する。 */
        [[nodiscard]] AlignedContinuousMemoryBlock allocate(Bytes size, Alignment alignment);

        /** allocateで返したブロックを返却し、可能ならbuddyと統合する。 */
        void deallocate(AlignedContinuousMemoryBlock block);

        /** BuddyAllocatorが管理するアリーナサイズを返す。 */
        [[nodiscard]] Bytes getArenaSize() const;

        ~BuddyAllocator();
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOCATOR_H
