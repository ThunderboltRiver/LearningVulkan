#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDER_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDER_H

#include "ResourceManagement/Memory/Bytes.h"

#include <cstdint>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * minBlockSize * 2^order のブロック階層を表す値。
     * BUDDY_ORDER_THRESHOLDで最大orderを制限しているため、内部表現は小さい整数で十分。
     * 値は生成後に変更せず、隣のorderが必要な場合は新しい値として作り直す。
     */
    struct BuddyOrder {
    private:
        std::uint8_t _value;

    public:
        explicit BuddyOrder(std::size_t value);

        [[nodiscard]] std::size_t value() const;

        [[nodiscard]] BuddyOrder next() const;

        [[nodiscard]] BuddyOrder previous() const;

        [[nodiscard]] Bytes bytesFor(Bytes minBlockSize) const;

        friend bool operator==(BuddyOrder lhs, BuddyOrder rhs);

        friend bool operator<(BuddyOrder lhs, BuddyOrder rhs);

        friend bool operator<=(BuddyOrder lhs, BuddyOrder rhs);

        friend bool operator>(BuddyOrder lhs, BuddyOrder rhs);
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDER_H
