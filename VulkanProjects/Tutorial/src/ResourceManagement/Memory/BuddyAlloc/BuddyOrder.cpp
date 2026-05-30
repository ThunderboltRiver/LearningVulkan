#include "ResourceManagement/Memory/BuddyAlloc/BuddyOrder.h"

#include <limits>
#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    BuddyOrder::BuddyOrder(const std::size_t value) : _value(static_cast<std::uint8_t>(value)) {
        if (value > std::numeric_limits<std::uint8_t>::max()) {
            throw std::overflow_error("BuddyOrder: value overflow");
        }
    }

    std::size_t BuddyOrder::value() const {
        return _value;
    }

    BuddyOrder BuddyOrder::next() const {
        return BuddyOrder{value() + 1};
    }

    BuddyOrder BuddyOrder::previous() const {
        if (_value == 0) {
            throw std::underflow_error("BuddyOrder: value underflow");
        }
        return BuddyOrder{value() - 1};
    }

    Bytes BuddyOrder::bytesFor(const Bytes minBlockSize) const {
        Bytes result = minBlockSize;
        for (std::size_t i = 0; i < value(); ++i) {
            result = result * 2;
        }
        return result;
    }

    bool operator==(const BuddyOrder lhs, const BuddyOrder rhs) {
        return lhs._value == rhs._value;
    }

    bool operator<(const BuddyOrder lhs, const BuddyOrder rhs) {
        return lhs._value < rhs._value;
    }

    bool operator<=(const BuddyOrder lhs, const BuddyOrder rhs) {
        return lhs._value <= rhs._value;
    }

    bool operator>(const BuddyOrder lhs, const BuddyOrder rhs) {
        return lhs._value > rhs._value;
    }
}
