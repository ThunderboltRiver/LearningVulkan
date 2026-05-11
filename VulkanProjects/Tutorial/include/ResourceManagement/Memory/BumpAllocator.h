#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUMPALLOCATOR_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUMPALLOCATOR_H

#include "ResourceManagement/Memory/MemoryBlock.h"

namespace Tutorial::ResourceManagement {

    /**
     * BuddyAllocatorへ供給する1つのアリーナ。
     * next は BumpAllocator が保持する侵入的リスト用リンク。
     */
    struct AlignedArena {
        AlignedContinuousMemoryBlock block;
        AlignedArena* next;
    };

    /**
     * 新しいアリーナをOSから追加取得するアロケータ。
     * 個別アリーナの返却は行わず、デストラクタでまとめてOSへ返す。
     */
    class BumpAllocator {
        /** 新規アリーナを確保するときの固定サイズ。 */
        Bytes _arenaSize;

        /** 確保済みアリーナをまとめて解放するための侵入的リスト先頭。 */
        AlignedArena* _arenaHead;

    public:
        explicit BumpAllocator(Bytes arenaSize = DEFAULT_ARENA_SIZE);

        BumpAllocator(const BumpAllocator&) = delete;
        BumpAllocator& operator=(const BumpAllocator&) = delete;

        /** 指定アライメントを満たす新しいアリーナを確保し、内部リストへ登録する。 */
        [[nodiscard]] AlignedArena* allocateArena(Alignment alignment);

        /** このBumpAllocatorが供給するアリーナの固定サイズを返す。 */
        [[nodiscard]] Bytes getArenaSize() const;

        ~BumpAllocator();
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUMPALLOCATOR_H
