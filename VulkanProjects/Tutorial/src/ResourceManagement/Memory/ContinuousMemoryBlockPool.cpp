#include "ResourceManagement/Memory/ContinuousMemoryBlockPool.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement {

    ContinuousMemoryBlockPool::ContinuousMemoryBlockPool(const Bytes arenaSize)
        : _buddyAllocator(arenaSize), _largeSizeAllocator() {
    }

    AlignedContinuousMemoryBlock ContinuousMemoryBlockPool::allocate(const Bytes size, const Alignment alignment) {
        if (size.isZero()) {
            throw std::invalid_argument("ContinuousMemoryBlockPool: size must be greater than 0");
        }
        if (size <= getArenaSize()) {
            return _buddyAllocator.allocate(size, alignment);
        }
        return _largeSizeAllocator.allocate(size, alignment);
    }

    void ContinuousMemoryBlockPool::deallocate(const AlignedContinuousMemoryBlock block) {
        if (block.ptr == nullptr) {
            throw std::invalid_argument("ContinuousMemoryBlockPool: block ptr must not be null");
        }
        if (block.size <= getArenaSize()) {
            _buddyAllocator.deallocate(block);
            return;
        }
        _largeSizeAllocator.deallocate(block);
    }

    Bytes ContinuousMemoryBlockPool::getArenaSize() const {
        return _buddyAllocator.getArenaSize();
    }
}
