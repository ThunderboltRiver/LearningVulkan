#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNMENT_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNMENT_H

#include "ResourceManagement/Memory/Bytes.h"

#include <cstdint>

namespace Tutorial::ResourceManagement::Memory {

    /**
     * アライメントを表す値オブジェクト。
     * BuddyAllocatorのアドレス計算は2の冪乗アライメントを前提にするため、
     * 2の冪乗でない値は丸めずに拒否する。
     */
    struct Alignment {
    private:
        Bytes _bytes;

    public:
        explicit Alignment(Bytes bytes);

        [[nodiscard]] Bytes bytes() const;

        /** address をこのAlignmentの倍数境界まで切り上げる。 */
        [[nodiscard]] std::uintptr_t alignUp(std::uintptr_t address) const;

        [[nodiscard]] bool equals(Alignment other) const;
    };

    bool operator==(Alignment lhs, Alignment rhs);

    bool operator!=(Alignment lhs, Alignment rhs);
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNMENT_H
