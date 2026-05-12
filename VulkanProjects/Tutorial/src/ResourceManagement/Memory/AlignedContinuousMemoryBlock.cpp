#include "ResourceManagement/Memory/AlignedContinuousMemoryBlock.h"

namespace Tutorial::ResourceManagement::Memory {

    bool AlignedContinuousMemoryBlock::isNull() const {
        return ptr == nullptr;
    }
}
