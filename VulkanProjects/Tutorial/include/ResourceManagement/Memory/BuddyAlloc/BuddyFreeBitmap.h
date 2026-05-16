#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYFREEBITMAP_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYFREEBITMAP_H

#include "ResourceManagement/Memory/BuddyAlloc/BuddyBlockIndex.h"

#include <cstddef>
#include <cstdint>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * あるorderに属するブロック群の空き状態を表すbitmap。
     * bit が 1 のとき、その BuddyBlockIndex のブロックが free list 上に存在する。
     * ArenaState の freeLists と同期して更新し、buddy 統合時の free 判定を高速化する。
     */
    struct BuddyFreeBitmap {

    private:
        /** bitmap word 配列。word は uint64_t 1要素を指し、64個分のblock状態bitを格納する単位。 */
        std::uint64_t* words;

        /** words に確保されている bitmap word 数。デストラクタとindex計算の境界情報として使う。 */
        std::size_t wordCount;

    public:
        /** 配列要素として未初期化orderを持てるようにするための空bitmap。 */
        BuddyFreeBitmap();

        /** blockCount 個のブロック状態を表せるだけのbitmap領域を0初期化して確保する。 */
        explicit BuddyFreeBitmap(std::size_t blockCount);

        /** words の二重解放を避けるためコピーは禁止する。 */
        BuddyFreeBitmap(const BuddyFreeBitmap&) = delete;
        BuddyFreeBitmap& operator=(const BuddyFreeBitmap&) = delete;

        /** ArenaState の固定配列へ後から構築済みbitmapを代入できるよう、所有権の移動だけ許可する。 */
        BuddyFreeBitmap(BuddyFreeBitmap&& other) noexcept;
        BuddyFreeBitmap& operator=(BuddyFreeBitmap&& other) noexcept;

        /** index のブロックが free list 上に存在するかを返す。 */
        [[nodiscard]] bool isFree(BuddyBlockIndex index) const;

        /** index のブロックが free list 上に存在するかどうかをbitmapへ反映する。 */
        void setFree(BuddyBlockIndex index, bool value) const;

        /** words を解放する。nullptr の場合は何もしない。 */
        ~BuddyFreeBitmap();
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYFREEBITMAP_H
