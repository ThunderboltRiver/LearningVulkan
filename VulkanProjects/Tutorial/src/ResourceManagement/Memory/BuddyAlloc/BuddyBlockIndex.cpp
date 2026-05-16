#include "ResourceManagement/Memory/BuddyAlloc/BuddyBlockIndex.h"

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    BuddyBlockIndex::BuddyBlockIndex(const std::size_t value) : _value(value) {
    }

    std::size_t BuddyBlockIndex::value() const {
        return _value;
    }

    BuddyBlockIndex BuddyBlockIndex::buddy() const {
        return BuddyBlockIndex{_value ^ 1};
    }

    BuddyBlockIndex BuddyBlockIndex::parent() const {
        return BuddyBlockIndex{_value / 2};
    }
}
