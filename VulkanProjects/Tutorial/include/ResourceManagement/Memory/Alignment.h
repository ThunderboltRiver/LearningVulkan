#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNMENT_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNMENT_H

#include "ResourceManagement/Memory/Bytes.h"

#include <cstdint>
#include <stdexcept>

namespace Tutorial::ResourceManagement {

    /**
     * アライメントを表す値オブジェクト。
     * 渡されたバイト数以上の最小の2の冪乗に丸めて保持する。
     */
    struct Alignment {
    private:
        Bytes _bytes;

    public:
        explicit constexpr Alignment(const Bytes bytes)
            : _bytes(bytes.roundUpToPowerOfTwo()) {
        }

        [[nodiscard]] constexpr Bytes bytes() const {
            return _bytes;
        }

        /** address をこのAlignmentの倍数境界まで切り上げる。 */
        [[nodiscard]] constexpr std::uintptr_t alignUp(const std::uintptr_t address) const {
            const auto mask = static_cast<std::uintptr_t>(_bytes.value() - 1);
            if (address > UINTPTR_MAX - mask) {
                throw std::overflow_error("Alignment: address alignment overflow");
            }
            return (address + mask) & ~mask;
        }

        friend constexpr bool operator==(const Alignment lhs, const Alignment rhs) {
            return lhs._bytes == rhs._bytes;
        }

        friend constexpr bool operator!=(const Alignment lhs, const Alignment rhs) {
            return !(lhs == rhs);
        }
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNMENT_H
