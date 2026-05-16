#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_CONTINUOUSMEMORYBLOCKPOOL_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_CONTINUOUSMEMORYBLOCKPOOL_H

#include "ResourceManagement/Memory/BuddyAlloc/BuddyAllocator.h"
#include "ResourceManagement/Memory/LargeSizeAllocator.h"

namespace Tutorial::ResourceManagement::Memory {

    /**
     * 連続メモリブロック要求の入口。
     * アリーナサイズ以下はBuddyAllocatorへ、それを超える要求はLargeSizeAllocatorへ委譲する。
     */
    class ContinuousMemoryBlockPool {
        /** アリーナサイズ以下の要求を処理するバディアロケータ。 */
        BuddyAlloc::BuddyAllocator _buddyAllocator;

        /** アリーナサイズを超える要求を処理する直接OS確保アロケータ。 */
        LargeSizeAllocator _largeSizeAllocator;

    public:
        explicit ContinuousMemoryBlockPool(Bytes arenaSize = DEFAULT_ARENA_SIZE);

        ContinuousMemoryBlockPool(const ContinuousMemoryBlockPool&) = delete;
        ContinuousMemoryBlockPool& operator=(const ContinuousMemoryBlockPool&) = delete;

        /** sizeバイト以上かつalignmentを満たす連続メモリブロックを確保する。 */
        [[nodiscard]] AlignedContinuousMemoryBlock allocate(Bytes size, Alignment alignment);

        /** allocateで取得したブロックを、サイズに応じた元アロケータへ返却する。 */
        void deallocate(AlignedContinuousMemoryBlock block);

        /** BuddyAllocator側で使うアリーナサイズを返す。 */
        [[nodiscard]] Bytes getArenaSize() const;
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_CONTINUOUSMEMORYBLOCKPOOL_H
