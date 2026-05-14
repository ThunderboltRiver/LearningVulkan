#include "ResourceManagement/Memory/BuddyAlloc/ArenaState.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {
    namespace {
        constexpr std::size_t BITMAP_WORD_BITS = sizeof(std::uint64_t) * 8;

        [[nodiscard]] Bytes bytesForOrder(const BuddyOrder order, const Bytes minBlockSize) {
            Bytes result = minBlockSize;
            for (std::size_t i = 0; i < order.value; ++i) {
                result = result * 2;
            }
            return result;
        }
    }

    ArenaState::ArenaState(
        BumpAlloc::AlignedArena* arena,
        const BuddyOrder maxOrder,
        const Bytes minBlockSize
    ) : arena(arena), freeLists{}, freeBitmaps{}, next(nullptr) {
        for (BuddyOrder order{0}; order <= maxOrder; ++order.value) {
            freeLists[order.value] = nullptr;
            const std::size_t blockCount = arena->block.size.value() / bytesForOrder(order, minBlockSize).value();
            const std::size_t wordCount = (blockCount + BITMAP_WORD_BITS - 1) / BITMAP_WORD_BITS;
            freeBitmaps[order.value] = BuddyFreeBitmap{
                new std::uint64_t[wordCount]{},
                wordCount
            };
        }

        auto* block = static_cast<FreeBlock*>(arena->block.ptr);
        block->next = nullptr;
        freeLists[maxOrder.value] = block;
        freeBitmaps[maxOrder.value].setFree(BuddyBlockIndex{0}, true);
    }

    void ArenaState::setNext(ArenaState* nextArenaState) {
        next = nextArenaState;
    }

    ArenaState::~ArenaState() {
        for (std::size_t order = 0; order < BUDDY_ORDER_COUNT; ++order) {
            delete[] freeBitmaps[order].words;
        }
    }
}
