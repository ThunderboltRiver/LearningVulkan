#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_MEMORYCONSTANTS_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_MEMORYCONSTANTS_H

#include "ResourceManagement/Memory/Bytes.h"

namespace Tutorial::ResourceManagement {

    /** free listノードを置ける最小ブロックサイズ。 */
    constexpr Bytes MIN_MEMORY_BLOCK_SIZE = Bytes::fromSizeT(sizeof(void*) * 2);

    /** BuddyAllocatorへ供給する既定アリーナサイズ。 */
    constexpr Bytes DEFAULT_ARENA_SIZE = Bytes::fromMiB(32);
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_MEMORYCONSTANTS_H
