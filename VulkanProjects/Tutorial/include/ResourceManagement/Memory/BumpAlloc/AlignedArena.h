#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUMPALLOC_ALIGNEDARENA_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUMPALLOC_ALIGNEDARENA_H

#include "ResourceManagement/Memory/AlignedContinuousMemoryBlock.h"

namespace Tutorial::ResourceManagement::Memory::BumpAlloc {

    /**
     * BumpAllocatorがBuddyAllocatorへ供給する1つのアリーナ。
     * next は BumpAllocator が保持する侵入的リスト用リンク。
     */
    struct AlignedArena {
        AlignedContinuousMemoryBlock block;
        AlignedArena* next;
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUMPALLOC_ALIGNEDARENA_H
