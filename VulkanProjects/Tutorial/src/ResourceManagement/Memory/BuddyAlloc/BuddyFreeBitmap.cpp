#include "ResourceManagement/Memory/BuddyAlloc/BuddyFreeBitmap.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /** uint64_t 1要素で管理できるブロック数。 */
    constexpr std::size_t BITMAP_WORD_BITS = sizeof(std::uint64_t) * 8;

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
        // 移動元のデストラクタが words を解放しないよう、所有権を空にする。
        other.words = nullptr;
        other.wordCount = 0;
    }

    BuddyFreeBitmap& BuddyFreeBitmap::operator=(BuddyFreeBitmap&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] words;
        // 既存の所有領域を捨ててから、移動元のbitmap領域を引き継ぐ。
        words = other.words;
        wordCount = other.wordCount;
        other.words = nullptr;
        other.wordCount = 0;
        return *this;
    }

    bool BuddyFreeBitmap::isFree(const BuddyBlockIndex index) const {
        const auto bitmapWordIndex = index.value() / BITMAP_WORD_BITS;
        const auto bitIndexInWord = index.value() % BITMAP_WORD_BITS;
        return (words[bitmapWordIndex] & (std::uint64_t{1} << bitIndexInWord)) != 0;
    }

    void BuddyFreeBitmap::setFree(const BuddyBlockIndex index, const bool value) const {
        // isFree と同じ位置計算を使い、対象bitだけを立てる/落とす。
        const auto bitmapWordIndex = index.value() / BITMAP_WORD_BITS;
        const auto bitIndexInWord = index.value() % BITMAP_WORD_BITS;
        const auto mask = std::uint64_t{1} << bitIndexInWord;
        if (value) {
            words[bitmapWordIndex] |= mask;
        } else {
            words[bitmapWordIndex] &= ~mask;
        }
    }

    BuddyFreeBitmap::~BuddyFreeBitmap() {
        delete[] words;
    }
}
