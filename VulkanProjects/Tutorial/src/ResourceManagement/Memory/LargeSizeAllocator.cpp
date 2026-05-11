#include "ResourceManagement/Memory/LargeSizeAllocator.h"

#include "ResourceManagement/Memory/OsMemory.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement {

    AlignedContinuousMemoryBlock LargeSizeAllocator::allocate(const Bytes size, const Alignment alignment) const {
        if (size.isZero()) {
            throw std::invalid_argument("LargeSizeAllocator: size must be greater than 0");
        }
        return allocateAlignedMemoryBlock(size.roundUpToPowerOfTwo(), alignment);
    }

    void LargeSizeAllocator::deallocate(const AlignedContinuousMemoryBlock block) const {
        if (block.ptr == nullptr) {
            throw std::invalid_argument("LargeSizeAllocator: block ptr must not be null");
        }
        deallocateAlignedMemoryBlock(block);
    }
}
