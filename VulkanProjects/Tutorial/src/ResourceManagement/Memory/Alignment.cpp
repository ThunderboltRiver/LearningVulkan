#include "ResourceManagement/Memory/Alignment.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory {

    Alignment::Alignment(const Bytes bytes) : _bytes(bytes) {
        if (!bytes.isPowerOfTwo()) {
            throw std::invalid_argument("Alignment: bytes must be a non-zero power of two");
        }
    }

    Bytes Alignment::bytes() const {
        return _bytes;
    }

    std::uintptr_t Alignment::alignUp(const std::uintptr_t address) const {
        const auto mask = static_cast<std::uintptr_t>(_bytes.value() - 1);
        if (address > UINTPTR_MAX - mask) {
            throw std::overflow_error("Alignment: address alignment overflow");
        }
        return (address + mask) & ~mask;
    }

    bool Alignment::equals(const Alignment other) const {
        return _bytes == other._bytes;
    }

    bool operator==(const Alignment lhs, const Alignment rhs) {
        return lhs.equals(rhs);
    }

    bool operator!=(const Alignment lhs, const Alignment rhs) {
        return !(lhs == rhs);
    }
}
