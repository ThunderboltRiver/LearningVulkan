#include "ResourceManagement/Memory/BuddyAlloc/BuddyFreeBitmap.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    bool BuddyFreeBitmap::isFree(const BuddyBlockIndex index) const {
        const auto word = index.value / (sizeof(std::uint64_t) * 8);
        const auto bit = index.value % (sizeof(std::uint64_t) * 8);
        return (words[word] & (std::uint64_t{1} << bit)) != 0;
    }

    void BuddyFreeBitmap::setFree(const BuddyBlockIndex index, const bool value) const {
        const auto word = index.value / (sizeof(std::uint64_t) * 8);
        const auto bit = index.value % (sizeof(std::uint64_t) * 8);
        const auto mask = std::uint64_t{1} << bit;
        if (value) {
            words[word] |= mask;
        } else {
            words[word] &= ~mask;
        }
    }
}
