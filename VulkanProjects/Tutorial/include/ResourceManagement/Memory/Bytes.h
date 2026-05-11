#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BYTES_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BYTES_H

#include <cstddef>
#include <limits>
#include <stdexcept>

namespace Tutorial::ResourceManagement {

    /**
     * バイト数を表す値オブジェクト。
     * メモリサイズの計算をstd::size_tの裸値から切り離し、演算時のoverflow/underflowを検出する。
     */
    struct Bytes {
    private:
        /** 実際のバイト数。外部APIでは直接受け渡しせず、Bytesとして扱う。 */
        std::size_t _value;

        explicit constexpr Bytes(const std::size_t value) : _value(value) {
        }

    public:
        /** std::size_tのバイト値からBytesを作成する。 */
        static constexpr Bytes fromSizeT(const std::size_t value) {
            return Bytes(value);
        }

        /** KiB単位の値からBytesを作成する。 */
        static constexpr Bytes fromKiB(const std::size_t value) {
            if (value > std::numeric_limits<std::size_t>::max() / 1024) {
                throw std::overflow_error("Bytes: KiB value overflow");
            }
            return Bytes(value * 1024);
        }

        /** MiB単位の値からBytesを作成する。 */
        static constexpr Bytes fromMiB(const std::size_t value) {
            if (value > std::numeric_limits<std::size_t>::max() / (1024 * 1024)) {
                throw std::overflow_error("Bytes: MiB value overflow");
            }
            return Bytes(value * 1024 * 1024);
        }

        /** 低レベルAPIへ渡すための裸のバイト数を返す。 */
        [[nodiscard]] constexpr std::size_t value() const {
            return _value;
        }

        /** 値が0バイトかどうかを返す。 */
        [[nodiscard]] constexpr bool isZero() const {
            return _value == 0;
        }

        /** 値が0以外の2の冪乗かどうかを返す。 */
        [[nodiscard]] constexpr bool isPowerOfTwo() const {
            return _value != 0 && (_value & (_value - 1)) == 0;
        }

        /** この値以上の最小の2の冪乗へ丸める。 */
        [[nodiscard]] constexpr Bytes roundUpToPowerOfTwo() const {
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

        /** rhsと比較して大きい方のBytesを返す。 */
        [[nodiscard]] constexpr Bytes max(const Bytes rhs) const {
            return *this >= rhs ? *this : rhs;
        }

        friend constexpr bool operator==(const Bytes lhs, const Bytes rhs) {
            return lhs._value == rhs._value;
        }

        friend constexpr bool operator!=(const Bytes lhs, const Bytes rhs) {
            return !(lhs == rhs);
        }

        friend constexpr bool operator<(const Bytes lhs, const Bytes rhs) {
            return lhs._value < rhs._value;
        }

        friend constexpr bool operator<=(const Bytes lhs, const Bytes rhs) {
            return lhs._value <= rhs._value;
        }

        friend constexpr bool operator>(const Bytes lhs, const Bytes rhs) {
            return lhs._value > rhs._value;
        }

        friend constexpr bool operator>=(const Bytes lhs, const Bytes rhs) {
            return lhs._value >= rhs._value;
        }

        /** バイト数同士を加算する。結果がstd::size_tを超える場合は例外。 */
        friend constexpr Bytes operator+(const Bytes lhs, const Bytes rhs) {
            if (lhs._value > std::numeric_limits<std::size_t>::max() - rhs._value) {
                throw std::overflow_error("Bytes: addition overflow");
            }
            return Bytes(lhs._value + rhs._value);
        }

        /** バイト数同士を減算する。負になる場合は例外。 */
        friend constexpr Bytes operator-(const Bytes lhs, const Bytes rhs) {
            if (lhs._value < rhs._value) {
                throw std::underflow_error("Bytes: subtraction underflow");
            }
            return Bytes(lhs._value - rhs._value);
        }

        /** バイト数を整数倍する。結果がstd::size_tを超える場合は例外。 */
        friend constexpr Bytes operator*(const Bytes lhs, const std::size_t rhs) {
            if (rhs != 0 && lhs._value > std::numeric_limits<std::size_t>::max() / rhs) {
                throw std::overflow_error("Bytes: multiplication overflow");
            }
            return Bytes(lhs._value * rhs);
        }

        friend constexpr Bytes operator*(const std::size_t lhs, const Bytes rhs) {
            return rhs * lhs;
        }

        /** バイト数を整数で割る。 */
        friend constexpr Bytes operator/(const Bytes lhs, const std::size_t rhs) {
            if (rhs == 0) {
                throw std::invalid_argument("Bytes: division by zero");
            }
            return Bytes(lhs._value / rhs);
        }

        /** lhsがrhsの何個分かを返す。 */
        friend constexpr std::size_t operator/(const Bytes lhs, const Bytes rhs) {
            if (rhs._value == 0) {
                throw std::invalid_argument("Bytes: division by zero");
            }
            return lhs._value / rhs._value;
        }

        /** バイト数同士の剰余を返す。 */
        friend constexpr Bytes operator%(const Bytes lhs, const Bytes rhs) {
            if (rhs._value == 0) {
                throw std::invalid_argument("Bytes: modulo by zero");
            }
            return Bytes(lhs._value % rhs._value);
        }

        Bytes& operator+=(const Bytes rhs) {
            *this = *this + rhs;
            return *this;
        }

        Bytes& operator-=(const Bytes rhs) {
            *this = *this - rhs;
            return *this;
        }
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BYTES_H
