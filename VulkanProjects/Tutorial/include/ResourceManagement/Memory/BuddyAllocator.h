#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOCATOR_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOCATOR_H

#include "ResourceManagement/Memory/BumpAllocator.h"

#include <cstddef>
#include <cstdint>

namespace Tutorial::ResourceManagement {

    /**
     * アリーナを2の冪乗サイズのブロックへ分割・統合して管理するアロケータ。
     * アライメントごとに独立したアリーナ群を持ち、free list と bitmap を同期して空き状態を管理する。
     */
    class BuddyAllocator {
        /**
         * 解放済みブロックの先頭領域をそのままリストノードとして使うための侵入的ノード。
         */
        struct FreeBlock {
            FreeBlock* next;
        };

        /**
         * order は minBlockSize * 2^order のブロック階層を表す値。
         * 最大32階層に制限しているため、内部表現は小さい整数で十分。
         */
        struct BuddyOrder {
            std::uint8_t value;

            friend bool operator==(const BuddyOrder lhs, const BuddyOrder rhs) {
                return lhs.value == rhs.value;
            }

            friend bool operator<(const BuddyOrder lhs, const BuddyOrder rhs) {
                return lhs.value < rhs.value;
            }

            friend bool operator<=(const BuddyOrder lhs, const BuddyOrder rhs) {
                return lhs.value <= rhs.value;
            }

            friend bool operator>(const BuddyOrder lhs, const BuddyOrder rhs) {
                return lhs.value > rhs.value;
            }
        };

        /**
         * あるorder内でのブロック番号。
         * アリーナ内ブロック数に比例するため値域はstd::size_tで保持する。
         */
        struct BuddyBlockIndex {
            std::size_t value;
        };

        /**
         * 1つのアリーナに紐づくBuddyAllocator用メタデータ。
         * freeLists と bitmaps は同じ空き状態を表し、更新は helper 経由で同期する。
         */
        struct ArenaState {
            /** このメタデータが管理する実アリーナ。 */
            AlignedArena* arena;

            /** orderごとの空きブロックリスト。ブロック本体の先頭をFreeBlockとして使う。 */
            FreeBlock* freeLists[32];

            /** orderごとの空き状態bitmap。1なら該当indexのブロックがfree list上に存在する。 */
            std::uint64_t* bitmaps[32];

            /** 各orderのbitmapに確保したuint64_t要素数。デストラクタと境界確認用のメタデータ。 */
            std::size_t bitmapWordCounts[32];

            /** 同じAlignmentを持つ次のアリーナメタデータ。 */
            ArenaState* next;
        };

        /**
         * 同じAlignmentを要求するアリーナ群。
         * アライメント別に分けることで、返却時に block.alignment から所属先を特定できる。
         */
        struct AlignedBuddyAllocator {
            Alignment alignment;
            ArenaState* arenas;
            AlignedBuddyAllocator* next;
        };

        BumpAllocator _bumpAllocator;

        /** Alignmentごとに分かれたBuddyAllocator状態の侵入的リスト先頭。 */
        AlignedBuddyAllocator* _alignedAllocators;

        /** order 0 に対応する最小ブロックサイズ。 */
        Bytes _minBlockSize;

        /** このアリーナサイズで取り得る最大order。arenaSize == minBlockSize * 2^_maxOrder。 */
        BuddyOrder _maxOrder;

        [[nodiscard]] AlignedBuddyAllocator* getOrCreateAlignedAllocator(Alignment alignment);

        [[nodiscard]] ArenaState* createArena(AlignedBuddyAllocator& allocator);

        [[nodiscard]] BuddyOrder orderFor(Bytes size) const;

        [[nodiscard]] Bytes bytesForOrder(BuddyOrder order) const;

        [[nodiscard]] ArenaState* findArenaContaining(AlignedBuddyAllocator& allocator, void* ptr) const;

        [[nodiscard]] bool isBlockFree(const ArenaState& arena, BuddyOrder order, BuddyBlockIndex index) const;

        void setBlockFree(ArenaState& arena, BuddyOrder order, BuddyBlockIndex index, bool value) const;

        void pushFreeBlock(ArenaState& arena, BuddyOrder order, BuddyBlockIndex index);

        [[nodiscard]] FreeBlock* removeFreeBlock(ArenaState& arena, BuddyOrder order, BuddyBlockIndex index);

        [[nodiscard]] BuddyBlockIndex blockIndex(const ArenaState& arena, const void* ptr, BuddyOrder order) const;

        [[nodiscard]] void* ptrForIndex(const ArenaState& arena, BuddyOrder order, BuddyBlockIndex index) const;

        void destroyMetadata() noexcept;

    public:
        explicit BuddyAllocator(Bytes arenaSize = DEFAULT_ARENA_SIZE, Bytes minBlockSize = MIN_MEMORY_BLOCK_SIZE);

        BuddyAllocator(const BuddyAllocator&) = delete;
        BuddyAllocator& operator=(const BuddyAllocator&) = delete;

        /** size以上かつalignmentを満たす最小orderのブロックを確保する。 */
        [[nodiscard]] AlignedContinuousMemoryBlock allocate(Bytes size, Alignment alignment);

        /** allocateで返したブロックを返却し、可能ならbuddyと統合する。 */
        void deallocate(AlignedContinuousMemoryBlock block);

        /** BuddyAllocatorが管理するアリーナサイズを返す。 */
        [[nodiscard]] Bytes getArenaSize() const;

        ~BuddyAllocator();
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOCATOR_H
