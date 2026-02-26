//
// Created by 沖田大河 on 2026/02/21.
//

#ifndef TUTORIAL_SPAN_H
#define TUTORIAL_SPAN_H

#include "PlacementStackAllocator.h"
#include "SpanAllocator.h"

template<typename T>
struct Span {
    T* const headPtr;
    const uint32_t count;

    T& operator [](const uint32_t index) const {
        if (index >= count) {
            throw std::out_of_range("Span: index out of range");
        }
        return headPtr[index];
    }

    static Span stackAlloc(const uint32_t count) {
        const auto allocResult = SpanAllocator::getAllocator()->stackAlloc<T>(count);
        return Span(allocResult);
    }

    Span& operator=(const Span& other) = delete;
    Span(const Span& other) = delete;

    ~Span() {
        for (uint32_t i = 0; i < count; ++i) {
            headPtr[count - 1 - i].~T();
        }
        SpanAllocator::getAllocator()->dealloc(_allocatedBytes);
    }

    template<typename U>
    friend struct Span;

private:
    const size_t _allocatedBytes;
    inline static PlacementStackAllocator* _allocator = nullptr;

    explicit Span(PlacementStackAllocator::AllocResult<T> allocResult):
        headPtr(allocResult.headPtr),
        count(allocResult.count),
        _allocatedBytes(allocResult.allocatedBytes) {
    }
};

#endif //TUTORIAL_SPAN_H