#include "ResourceManagement/Memory/BuddyAlloc/BuddyOrder.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    bool operator==(const BuddyOrder lhs, const BuddyOrder rhs) {
        return lhs.value == rhs.value;
    }

    bool operator<(const BuddyOrder lhs, const BuddyOrder rhs) {
        return lhs.value < rhs.value;
    }

    bool operator<=(const BuddyOrder lhs, const BuddyOrder rhs) {
        return lhs.value <= rhs.value;
    }

    bool operator>(const BuddyOrder lhs, const BuddyOrder rhs) {
        return lhs.value > rhs.value;
    }
}
