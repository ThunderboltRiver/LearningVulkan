#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_MEMORYCONSTANTS_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_MEMORYCONSTANTS_H

#include "ResourceManagement/Memory/Bytes.h"

namespace Tutorial::ResourceManagement::Memory {

    /** free listノードを置ける最小ブロックサイズ。 */
    extern const Bytes MIN_MEMORY_BLOCK_SIZE;

    /** BuddyAllocatorへ供給する既定アリーナサイズ。 */
    extern const Bytes DEFAULT_ARENA_SIZE;
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_MEMORYCONSTANTS_H
