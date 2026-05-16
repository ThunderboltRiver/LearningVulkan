#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_FREEBLOCK_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_FREEBLOCK_H

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * 解放済みブロックの先頭領域をそのままリストノードとして使うための侵入的ノード。
     * previous/next を持つ双方向リストにすることで、index から算出した node を O(1) で unlink できる。
     */
    struct FreeBlock {
        FreeBlock* previous;
        FreeBlock* next;
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_FREEBLOCK_H
