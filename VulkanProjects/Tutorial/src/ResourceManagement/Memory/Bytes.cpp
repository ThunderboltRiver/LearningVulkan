#include "ResourceManagement/Memory/Bytes.h"

#include <bit>
#include <limits>
#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory {

    Bytes::Bytes(const std::size_t value) : _value(value) {
    }

    Bytes Bytes::fromSizeT(const std::size_t value) {
        return Bytes(value);
    }

    Bytes Bytes::fromKiB(const std::size_t value) {
        if (value > std::numeric_limits<std::size_t>::max() / 1024) {
            throw std::overflow_error("Bytes: KiB value overflow");
        }
        return Bytes(value * 1024);
    }

    Bytes Bytes::fromMiB(const std::size_t value) {
        if (value > std::numeric_limits<std::size_t>::max() / (1024 * 1024)) {
            throw std::overflow_error("Bytes: MiB value overflow");
        }
        return Bytes(value * 1024 * 1024);
    }

    Bytes Bytes::fromPowerOfTwoExponent(const std::size_t exponent) {
        if (exponent >= std::numeric_limits<std::size_t>::digits) {
            throw std::overflow_error("Bytes: power-of-two exponent overflow");
        }
        return Bytes(static_cast<std::size_t>(1) << exponent);
    }

    Bytes Bytes::max(const Bytes lhs, const Bytes rhs) {
        return lhs >= rhs ? lhs : rhs;
    }

    std::size_t Bytes::value() const {
        return _value;
    }

    bool Bytes::isZero() const {
        return _value == 0;
    }

    bool Bytes::isPowerOfTwo() const {
        return _value != 0 && (_value & (_value - 1)) == 0;
    }

    Bytes Bytes::roundUpToPowerOfTwo() const {
        std::size_t value = _value;
        if (value == 0) {
            throw std::invalid_argument("Bytes: bytes must be greater than 0");
        }
        --value;
        for (std::size_t shift = 1; shift < sizeof(std::size_t) * 8; shift <<= 1) {
            value |= value >> shift;
        }
        if (value == static_cast<std::size_t>(-1)) {
            throw std::overflow_error("Bytes: power-of-two rounding overflow");
        }
        return Bytes(value + 1);
    }

    std::size_t Bytes::log2PowerOfTwo() const {
        if (!isPowerOfTwo()) {
            throw std::invalid_argument("Bytes: bytes must be a power of two");
        }
        return static_cast<std::size_t>(std::countr_zero(_value));
    }

    bool operator==(const Bytes lhs, const Bytes rhs) {
        return lhs._value == rhs._value;
    }

    bool operator!=(const Bytes lhs, const Bytes rhs) {
        return !(lhs == rhs);
    }

    bool operator<(const Bytes lhs, const Bytes rhs) {
        return lhs._value < rhs._value;
    }

    bool operator<=(const Bytes lhs, const Bytes rhs) {
        return lhs._value <= rhs._value;
    }

    bool operator>(const Bytes lhs, const Bytes rhs) {
        return lhs._value > rhs._value;
    }

    bool operator>=(const Bytes lhs, const Bytes rhs) {
        return lhs._value >= rhs._value;
    }

    Bytes operator+(const Bytes lhs, const Bytes rhs) {
        if (lhs._value > std::numeric_limits<std::size_t>::max() - rhs._value) {
            throw std::overflow_error("Bytes: addition overflow");
        }
        return Bytes(lhs._value + rhs._value);
    }

    Bytes operator-(const Bytes lhs, const Bytes rhs) {
        if (lhs._value < rhs._value) {
            throw std::underflow_error("Bytes: subtraction underflow");
        }
        return Bytes(lhs._value - rhs._value);
    }

    Bytes operator*(const Bytes lhs, const std::size_t rhs) {
        if (rhs != 0 && lhs._value > std::numeric_limits<std::size_t>::max() / rhs) {
            throw std::overflow_error("Bytes: multiplication overflow");
        }
        return Bytes(lhs._value * rhs);
    }

    Bytes operator*(const std::size_t lhs, const Bytes rhs) {
        return rhs * lhs;
    }

    Bytes operator/(const Bytes lhs, const std::size_t rhs) {
        if (rhs == 0) {
            throw std::invalid_argument("Bytes: division by zero");
        }
        return Bytes(lhs._value / rhs);
    }

    std::size_t operator/(const Bytes lhs, const Bytes rhs) {
        if (rhs._value == 0) {
            throw std::invalid_argument("Bytes: division by zero");
        }
        return lhs._value / rhs._value;
    }

    Bytes operator%(const Bytes lhs, const Bytes rhs) {
        if (rhs._value == 0) {
            throw std::invalid_argument("Bytes: modulo by zero");
        }
        return Bytes(lhs._value % rhs._value);
    }

    Bytes& Bytes::operator+=(const Bytes rhs) {
        *this = *this + rhs;
        return *this;
    }

    Bytes& Bytes::operator-=(const Bytes rhs) {
        *this = *this - rhs;
        return *this;
    }
}
