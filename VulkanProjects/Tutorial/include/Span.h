//
// Created by 沖田大河 on 2026/02/21.
//

#ifndef TUTORIAL_SPAN_H
#define TUTORIAL_SPAN_H

#include "PlacementStackAllocator.h"
#include "SpanAllocator.h"
#include <type_traits>

template<typename T>
struct Span {
    static_assert(std::is_trivially_destructible_v<T>, "Span only supports trivially destructible types");

    T* headPtr;
    uint32_t count;

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

    Span(Span&& other) noexcept :
        headPtr(other.headPtr),
        count(other.count),
        _allocator(other._allocator),
        _allocatedBytes(other._allocatedBytes) {
        other.headPtr = nullptr;
        other.count = 0;
        other._allocator = nullptr;
        other._allocatedBytes = 0;
    }

    Span& operator=(Span&& other) noexcept {
        if (this != &other) {
            if (headPtr != nullptr && _allocator != nullptr) {
                _allocator->dealloc(_allocatedBytes);
            }
            headPtr = other.headPtr;
            count = other.count;
            _allocator = other._allocator;
            _allocatedBytes = other._allocatedBytes;
            other.headPtr = nullptr;
            other.count = 0;
            other._allocator = nullptr;
            other._allocatedBytes = 0;
        }
        return *this;
    }

    ~Span() noexcept {
        if (headPtr != nullptr) {
            _allocator->dealloc(_allocatedBytes);
        }
    }

private:
    PlacementStackAllocator* _allocator;
    size_t _allocatedBytes;

    explicit Span(PlacementStackAllocator::AllocResult<T> allocResult):
        headPtr(allocResult.headPtr),
        count(allocResult.count),
        _allocator(SpanAllocator::getAllocator()),
        _allocatedBytes(allocResult.allocatedBytes) {
    }
};

#endif //TUTORIAL_SPAN_H