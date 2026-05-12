#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_FREEBLOCK_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_FREEBLOCK_H

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * 解放済みブロックの先頭領域をそのままリストノードとして使うための侵入的ノード。
     */
    struct FreeBlock {
        FreeBlock* next;
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_FREEBLOCK_H
