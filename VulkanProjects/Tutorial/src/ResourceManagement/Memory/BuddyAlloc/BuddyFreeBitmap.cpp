#include "ResourceManagement/Memory/BuddyAlloc/BuddyFreeBitmap.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {
    namespace {
        constexpr std::size_t BITMAP_WORD_BITS = sizeof(std::uint64_t) * 8;
    }

    BuddyFreeBitmap::BuddyFreeBitmap()
        : words(nullptr), wordCount(0) {
    }

    BuddyFreeBitmap::BuddyFreeBitmap(const std::size_t blockCount)
        : words(nullptr),
          wordCount((blockCount + BITMAP_WORD_BITS - 1) / BITMAP_WORD_BITS) {
        words = new std::uint64_t[wordCount]{};
    }

    BuddyFreeBitmap::BuddyFreeBitmap(BuddyFreeBitmap&& other) noexcept
        : words(other.words), wordCount(other.wordCount) {
        other.words = nullptr;
        other.wordCount = 0;
    }

    BuddyFreeBitmap& BuddyFreeBitmap::operator=(BuddyFreeBitmap&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] words;
        words = other.words;
        wordCount = other.wordCount;
        other.words = nullptr;
        other.wordCount = 0;
        return *this;
    }

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

    BuddyFreeBitmap::~BuddyFreeBitmap() {
        delete[] words;
    }
}
