#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDER_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDER_H

#include <cstdint>

namespace Tutorial::ResourceManagement::BuddyAlloc {

    /**
     * minBlockSize * 2^order のブロック階層を表す値。
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
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDER_H
