#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ALIGNEDBUDDYALLOCATOR_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ALIGNEDBUDDYALLOCATOR_H

#include "ResourceManagement/Memory/Alignment.h"
#include "ResourceManagement/Memory/BuddyAlloc/ArenaState.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * 同じAlignmentを要求するアリーナ群。
     * アライメント別に分けることで、返却時に block.alignment から所属先を特定できる。
     */
    struct AlignedBuddyAllocator {
        Alignment alignment;
        ArenaState* arenas;
        AlignedBuddyAllocator* next;
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_ALIGNEDBUDDYALLOCATOR_H
