#include "ResourceManagement/Memory/BuddyAlloc/ArenaState.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    ArenaState::~ArenaState() {
        for (std::size_t order = 0; order < BUDDY_ORDER_COUNT; ++order) {
            delete[] freeBitmaps[order].words;
        }
    }
}
