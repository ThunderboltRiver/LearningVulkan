#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYBLOCKINDEX_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYBLOCKINDEX_H

#include <cstddef>

namespace Tutorial::ResourceManagement::BuddyAlloc {

    /**
     * あるorder内でのブロック番号。
     * アリーナ内ブロック数に比例するため値域はstd::size_tで保持する。
     */
    struct BuddyBlockIndex {
        std::size_t value;
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYBLOCKINDEX_H
