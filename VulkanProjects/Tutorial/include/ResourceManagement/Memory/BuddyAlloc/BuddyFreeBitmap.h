#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYFREEBITMAP_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYFREEBITMAP_H

#include "ResourceManagement/Memory/BuddyAlloc/BuddyBlockIndex.h"

#include <cstddef>
#include <cstdint>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * あるorderに属するブロック群の空き状態を表すbitmap。
     * bit が 1 のとき、その BuddyBlockIndex のブロックが free list 上に存在する。
     */
    struct BuddyFreeBitmap {
        std::uint64_t* words;
        std::size_t wordCount;

        explicit BuddyFreeBitmap(std::size_t blockCount);

        BuddyFreeBitmap(const BuddyFreeBitmap&) = delete;
        BuddyFreeBitmap& operator=(const BuddyFreeBitmap&) = delete;

        BuddyFreeBitmap(BuddyFreeBitmap&& other) noexcept;
        BuddyFreeBitmap& operator=(BuddyFreeBitmap&& other) noexcept;

        [[nodiscard]] bool isFree(BuddyBlockIndex index) const;

        void setFree(BuddyBlockIndex index, bool value) const;

        ~BuddyFreeBitmap();
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYFREEBITMAP_H
