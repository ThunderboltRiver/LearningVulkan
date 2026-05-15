#include "ResourceManagement/Memory/BuddyAlloc/BuddyOrder.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    Bytes BuddyOrder::bytesFor(const Bytes minBlockSize) const {
        Bytes result = minBlockSize;
        for (std::size_t i = 0; i < value; ++i) {
            result = result * 2;
        }
        return result;
    }

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
