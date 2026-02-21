//
// Created by 沖田大河 on 2026/02/19.
//

#ifndef TUTORIAL_PLACEMENTSTACKALLOCATOR_H
#define TUTORIAL_PLACEMENTSTACKALLOCATOR_H
#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <stdexcept>

constexpr std::size_t STACK_ALLOCATOR_1_MB_CAPACITY = 1024 * 1024;

class PlacementStackAllocator {
    const std::size_t _capacity;
    char* const _stackBottomElementPtr;
    std::size_t _occupied;

public:

    /**
     * 割り当ての結果
     * @tparam T 割り当てる型
     */
    template<typename T>
    struct AllocResult {
        /**
         * 割り当てられた領域の先頭へのポインタ
         */
        T* const headPtr;

        /**
         * 割り当てられた要素の数
         */
        const uint32_t count;

        /**
         * パディングも込みの割り当てられた領域のバイト数
         */
        const std::size_t allocatedBytes;

        AllocResult(T* const headPtr, const uint32_t count, const std::size_t allocatedBytes):
            headPtr(headPtr),
            count(count),
            allocatedBytes(allocatedBytes) {
            if (headPtr == nullptr) {
                throw std::runtime_error("PlacementStackAllocator: headPtr must not be null");
            }
            if (count < 1) {
                throw std::runtime_error("PlacementStackAllocator: count must be greater than 1");
            }
            if (allocatedBytes < sizeof(T) * static_cast<std::size_t>(count)) {
                throw std::runtime_error("PlacementStackAllocator: allocatedBytes must be at least sizeof(T) * count");
            }
        }
    };

    explicit PlacementStackAllocator(std::size_t capacity);

    PlacementStackAllocator(const PlacementStackAllocator&) = delete;
    PlacementStackAllocator& operator=(const PlacementStackAllocator&) = delete;

    template<typename T>
    [[nodiscard]] AllocResult<T> stackAlloc(uint32_t count = 1) {
        if (count == 0) {
            throw std::runtime_error("PlacementStackAllocator: count must be greater than 0");
        }

        // スタックの底から、すでに占有されている領域を飛ばした位置が、これから割り当てる領域の先頭になり得る
        char* tailPtr = _stackBottomElementPtr + _occupied;
        const auto tailAddress = reinterpret_cast<std::uintptr_t>(tailPtr);

        // アライメントを考慮してパディングを計算する
        std::uintptr_t padding = 0;
        if (tailAddress % alignof(T) != 0) {
            padding = alignof(T) - tailAddress % alignof(T);
        }

        // 必要となるメモリのバイト数を計算して、スタックの容量を超えるなら例外をスローする
        const auto requiredBytes = padding + sizeof(T) * static_cast<std::size_t>(count);
        if (_occupied + requiredBytes > _capacity) {
            throw std::runtime_error("PlacementStackAllocator: Out of memory");
        }

        // 占有されている領域のバイト数を更新して、割り当てる領域の先頭へのポインタを返す
        _occupied += requiredBytes;
        return AllocResult<T>(reinterpret_cast<T*>(tailPtr + padding), count, requiredBytes);
    }

    void dealloc(std::size_t allocatedBytes);

    ~PlacementStackAllocator();
};

#endif //TUTORIAL_PLACEMENTSTACKALLOCATOR_H