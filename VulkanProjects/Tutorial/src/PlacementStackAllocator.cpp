//
// Created by 沖田大河 on 2026/02/19.
//

#include "PlacementStackAllocator.h"
#include <cstdlib>
#include <stdexcept>

PlacementStackAllocator::PlacementStackAllocator(const std::size_t capacity):
    _capacity(capacity),
    _stackBottomElementPtr(static_cast<char*>(std::malloc(capacity))),
    _occupied(0) {
    if (_stackBottomElementPtr == nullptr) {
        throw std::bad_alloc();
    }
}

void PlacementStackAllocator::dealloc(const std::size_t allocatedBytes) {
    // 解放する領域のバイト数が占有を超えるなら例外
    if (allocatedBytes > _occupied) {
        throw std::runtime_error("PlacementStackAllocator: Cannot deallocate more bytes than currently occupied");
    }
    _occupied -= allocatedBytes;
}

PlacementStackAllocator::~PlacementStackAllocator() {
    std::free(_stackBottomElementPtr);
}
