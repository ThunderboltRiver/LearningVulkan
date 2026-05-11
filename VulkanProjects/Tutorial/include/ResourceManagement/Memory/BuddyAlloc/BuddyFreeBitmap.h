#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYFREEBITMAP_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYFREEBITMAP_H

#include "ResourceManagement/Memory/BuddyAlloc/BuddyBlockIndex.h"

#include <cstddef>
#include <cstdint>

namespace Tutorial::ResourceManagement::BuddyAlloc {

    /**
     * あるorderに属するブロック群の空き状態を表すbitmap。
     * bit が 1 のとき、その BuddyBlockIndex のブロックが free list 上に存在する。
     */
    struct BuddyFreeBitmap {
        std::uint64_t* words;
        std::size_t wordCount;

        [[nodiscard]] bool isFree(BuddyBlockIndex index) const {
            const auto word = index.value / (sizeof(std::uint64_t) * 8);
            const auto bit = index.value % (sizeof(std::uint64_t) * 8);
            return (words[word] & (std::uint64_t{1} << bit)) != 0;
        }

        void setFree(BuddyBlockIndex index, const bool value) const {
            const auto word = index.value / (sizeof(std::uint64_t) * 8);
            const auto bit = index.value % (sizeof(std::uint64_t) * 8);
            const auto mask = std::uint64_t{1} << bit;
            if (value) {
                words[word] |= mask;
            } else {
                words[word] &= ~mask;
            }
        }
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYFREEBITMAP_H
