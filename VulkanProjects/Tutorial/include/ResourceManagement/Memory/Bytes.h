#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BYTES_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BYTES_H

#include <cstddef>

namespace Tutorial::ResourceManagement::Memory {

    /**
     * バイト数を表す値オブジェクト。
     * メモリサイズの計算をstd::size_tの裸値から切り離し、演算時のoverflow/underflowを検出する。
     */
    struct Bytes {
    private:
        /** 実際のバイト数。外部APIでは直接受け渡しせず、Bytesとして扱う。 */
        std::size_t _value;

        explicit Bytes(std::size_t value);

    public:
        /** std::size_tのバイト値からBytesを作成する。 */
        static Bytes fromSizeT(std::size_t value);

        /** KiB単位の値からBytesを作成する。 */
        static Bytes fromKiB(std::size_t value);

        /** MiB単位の値からBytesを作成する。 */
        static Bytes fromMiB(std::size_t value);

        /** 2^exponent バイトのBytesを作成する。 */
        static Bytes fromPowerOfTwoExponent(std::size_t exponent);

        /** 2つのBytesのうち大きい方を返す。 */
        [[nodiscard]] static Bytes max(Bytes lhs, Bytes rhs);

        /** 低レベルAPIへ渡すための裸のバイト数を返す。 */
        [[nodiscard]] std::size_t value() const;

        /** 値が0バイトかどうかを返す。 */
        [[nodiscard]] bool isZero() const;

        /** 値が0以外の2の冪乗かどうかを返す。 */
        [[nodiscard]] bool isPowerOfTwo() const;

        /** この値以上の最小の2の冪乗へ丸める。 */
        [[nodiscard]] Bytes roundUpToPowerOfTwo() const;

        /** 2の冪乗値として見たときの指数を返す。 */
        [[nodiscard]] std::size_t log2PowerOfTwo() const;

        friend bool operator==(Bytes lhs, Bytes rhs);
        friend bool operator!=(Bytes lhs, Bytes rhs);
        friend bool operator<(Bytes lhs, Bytes rhs);
        friend bool operator<=(Bytes lhs, Bytes rhs);
        friend bool operator>(Bytes lhs, Bytes rhs);
        friend bool operator>=(Bytes lhs, Bytes rhs);
        friend Bytes operator+(Bytes lhs, Bytes rhs);
        friend Bytes operator-(Bytes lhs, Bytes rhs);
        friend Bytes operator*(Bytes lhs, std::size_t rhs);
        friend Bytes operator*(std::size_t lhs, Bytes rhs);
        friend Bytes operator/(Bytes lhs, std::size_t rhs);
        friend std::size_t operator/(Bytes lhs, Bytes rhs);
        friend Bytes operator%(Bytes lhs, Bytes rhs);

        Bytes& operator+=(Bytes rhs);

        Bytes& operator-=(Bytes rhs);
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BYTES_H
