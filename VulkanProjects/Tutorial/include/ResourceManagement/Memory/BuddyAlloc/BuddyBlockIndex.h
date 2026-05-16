#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYBLOCKINDEX_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYBLOCKINDEX_H

#include <cstddef>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /**
     * あるorder内でのブロック番号。
     * アリーナ内ブロック数に比例するため値域はstd::size_tで保持する。
     * 値は生成後に変更せず、buddyや親indexが必要な場合は新しい値として作り直す。
     */
    struct BuddyBlockIndex {
    private:
        std::size_t _value;

    public:
        explicit BuddyBlockIndex(std::size_t value);

        [[nodiscard]] std::size_t value() const;

        [[nodiscard]] BuddyBlockIndex buddy() const;

        [[nodiscard]] BuddyBlockIndex parent() const;
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYBLOCKINDEX_H
