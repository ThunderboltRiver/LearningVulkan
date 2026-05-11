#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_MEMORYBLOCK_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_MEMORYBLOCK_H

#include "ResourceManagement/Memory/Bytes.h"
#include <cstdint>
#include <stdexcept>

namespace Tutorial::ResourceManagement {

    /** free listノードを置ける最小ブロックサイズ。 */
    constexpr Bytes MIN_MEMORY_BLOCK_SIZE = Bytes::fromSizeT(sizeof(void*) * 2);

    /** BuddyAllocatorへ供給する既定アリーナサイズ。 */
    constexpr Bytes DEFAULT_ARENA_SIZE = Bytes::fromMiB(32);

    /** bytes が0以外の2の冪乗かどうかを返す。 */
    [[nodiscard]] constexpr bool isPowerOfTwo(const Bytes bytes) {
        const auto value = bytes.value();
        return value != 0 && (value & (value - 1)) == 0;
    }

    /** bytes 以上の最小の2の冪乗に丸める。 */
    [[nodiscard]] constexpr Bytes roundUpToPowerOfTwo(const Bytes bytes) {
        std::size_t value = bytes.value();
        if (value == 0) {
            throw std::invalid_argument("MemoryBlock: bytes must be greater than 0");
        }
        --value;
        for (std::size_t shift = 1; shift < sizeof(std::size_t) * 8; shift <<= 1) {
            value |= value >> shift;
        }
        if (value == static_cast<std::size_t>(-1)) {
            throw std::overflow_error("MemoryBlock: power-of-two rounding overflow");
        }
        return Bytes::fromSizeT(value + 1);
    }

    /** 2つのBytesのうち大きい方を返す。 */
    [[nodiscard]] constexpr Bytes maxBytes(const Bytes lhs, const Bytes rhs) {
        return lhs >= rhs ? lhs : rhs;
    }

    /**
     * アライメントを表す値オブジェクト。
     * 渡されたバイト数以上の最小の2の冪乗に丸めて保持する。
     */
    struct Alignment {
    private:
        Bytes _bytes;

    public:
        explicit constexpr Alignment(const Bytes bytes)
            : _bytes(roundUpToPowerOfTwo(bytes)) {
        }

        [[nodiscard]] constexpr Bytes bytes() const {
            return _bytes;
        }

        friend constexpr bool operator==(const Alignment lhs, const Alignment rhs) {
            return lhs._bytes == rhs._bytes;
        }

        friend constexpr bool operator!=(const Alignment lhs, const Alignment rhs) {
            return !(lhs == rhs);
        }
    };

    /**
     * 仮想アドレス空間上で連続した、指定アライメント済みのメモリブロック。
     * ptr は先頭アドレス、size は利用可能な領域サイズ、alignment は ptr が満たすアライメントを表す。
     */
    struct AlignedContinuousMemoryBlock {
        void* ptr;
        Bytes size;
        Alignment alignment;

        [[nodiscard]] bool isNull() const {
            return ptr == nullptr;
        }
    };

    /** address を alignment の倍数境界まで切り上げる。 */
    [[nodiscard]] constexpr std::uintptr_t alignUp(const std::uintptr_t address, const Alignment alignment) {
        const auto mask = static_cast<std::uintptr_t>(alignment.bytes().value() - 1);
        if (address > UINTPTR_MAX - mask) {
            throw std::overflow_error("MemoryBlock: address alignment overflow");
        }
        return (address + mask) & ~mask;
    }
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_MEMORYBLOCK_H
