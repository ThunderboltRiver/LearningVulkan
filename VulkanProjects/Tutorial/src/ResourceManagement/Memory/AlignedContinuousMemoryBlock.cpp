#include "ResourceManagement/Memory/AlignedContinuousMemoryBlock.h"

#include <cstdint>

namespace Tutorial::ResourceManagement::Memory {

    AlignedContinuousMemoryBlock AlignedContinuousMemoryBlock::empty(const Alignment alignment) {
        return AlignedContinuousMemoryBlock{
            nullptr,
            Bytes::fromSizeT(0),
            alignment
        };
    }

    bool AlignedContinuousMemoryBlock::isNull() const {
        return ptr == nullptr;
    }

    bool AlignedContinuousMemoryBlock::contains(const void* targetPtr) const {
        const auto targetAddress = reinterpret_cast<std::uintptr_t>(targetPtr);
        const auto beginAddress = reinterpret_cast<std::uintptr_t>(ptr);
        return targetAddress >= beginAddress && targetAddress < beginAddress + size.value();
    }
}
