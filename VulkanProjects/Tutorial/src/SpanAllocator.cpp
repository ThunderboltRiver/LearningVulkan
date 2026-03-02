//
// Created by 沖田大河 on 2026/02/26.
//

#include "SpanAllocator.h"

PlacementStackAllocator* SpanAllocator::getAllocator() {
    if (allocator == nullptr) {
        throw std::runtime_error("SpanAllocator: allocator must be set before calling getAllocator");
    }
    return allocator;
}

void SpanAllocator::setAllocator(PlacementStackAllocator* pAllocator) {
    if (pAllocator == nullptr) {
        throw std::runtime_error("SpanAllocator: allocator must not be null");
    }
    allocator = pAllocator;
}

void SpanAllocator::resetAllocator() noexcept{
    allocator = nullptr;
}
