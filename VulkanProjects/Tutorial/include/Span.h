//
// Created by 沖田大河 on 2026/02/21.
//

#ifndef TUTORIAL_DYNAMICARRAY_H
#define TUTORIAL_DYNAMICARRAY_H

#include "PlacementStackAllocator.h"

template<typename T>
struct Span {
    T* const headPtr;
    const uint32_t count;

    T operator [](const uint32_t index) const {
        if (index >= count) {
            throw std::out_of_range("Span: index out of range");
        }
        const auto result = headPtr[index];
        return result;
    }

    void static SetAllocator(PlacementStackAllocator* const allocator) {
        Span<void*>::_allocator = allocator;
    }

    void static ResetAllocator() {
        Span<void*>::_allocator = nullptr;
    }

    static Span stackAlloc(const uint32_t count) {
        const auto allocResult = getAllocator()->stackAlloc<T>(count);
        return Span(allocResult);
    }

    ~Span() {
        getAllocator()->dealloc(_allocatedBytes);
        for (uint32_t i = 0; i < count; ++i) {
            headPtr[count - 1 - i].~T();
        }
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

    static PlacementStackAllocator* getAllocator() {
        if (Span<void*>::_allocator == nullptr) {
            throw std::runtime_error("Span: allocator must be set before calling stackAlloc");
        }
        const auto allocator = Span<void*>::_allocator;
        return allocator;
    }
};

#endif //TUTORIAL_DYNAMICARRAY_H