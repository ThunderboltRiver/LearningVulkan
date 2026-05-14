#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDER_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDER_H

#include <cstdint>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * minBlockSize * 2^order のブロック階層を表す値。
     * BUDDY_ORDER_THRESHOLDで最大orderを制限しているため、内部表現は小さい整数で十分。
     */
    struct BuddyOrder {
        std::uint8_t value;

        friend bool operator==(BuddyOrder lhs, BuddyOrder rhs);

        friend bool operator<(BuddyOrder lhs, BuddyOrder rhs);

        friend bool operator<=(BuddyOrder lhs, BuddyOrder rhs);

        friend bool operator>(BuddyOrder lhs, BuddyOrder rhs);
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDER_H
