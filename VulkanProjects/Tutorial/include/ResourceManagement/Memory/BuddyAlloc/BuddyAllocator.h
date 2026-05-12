#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOCATOR_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOCATOR_H

#include "ResourceManagement/Memory/BuddyAlloc/AlignedBuddyAllocator.h"
#include "ResourceManagement/Memory/BuddyAlloc/BuddyBlockIndex.h"
#include "ResourceManagement/Memory/BuddyAlloc/BuddyOrder.h"
#include "ResourceManagement/Memory/BuddyAlloc/FreeBlock.h"
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

        /** order 0 に対応する最小ブロックサイズ。 */
        Bytes _minBlockSize;

        /** このアリーナサイズで取り得る最大order。arenaSize == minBlockSize * 2^_maxOrder。 */
        BuddyAlloc::BuddyOrder _maxOrder;

        [[nodiscard]] BuddyAlloc::AlignedBuddyAllocator* getOrCreateAlignedAllocator(Alignment alignment);

        [[nodiscard]] BuddyAlloc::AlignedBuddyAllocator* findAlignedAllocator(Alignment alignment) const;

        [[nodiscard]] BuddyAlloc::BuddyOrder orderFor(Bytes size) const;

        void destroyMetadata() noexcept;

    public:
        explicit BuddyAllocator(Bytes arenaSize = DEFAULT_ARENA_SIZE, Bytes minBlockSize = MIN_MEMORY_BLOCK_SIZE);

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
