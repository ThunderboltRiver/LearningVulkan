#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNEDCONTINUOUSMEMORYBLOCK_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNEDCONTINUOUSMEMORYBLOCK_H

#include "ResourceManagement/Memory/Alignment.h"

namespace Tutorial::ResourceManagement {

    /**
     * 仮想アドレス空間上で連続した、指定アライメント済みのメモリブロック。
     * ptr は先頭アドレス、size は利用可能な領域サイズ、alignment は ptr が満たすアライメントを表す。
     */
    struct AlignedContinuousMemoryBlock {
        void* ptr;
        Bytes size;
        Alignment alignment;

        [[nodiscard]] bool isNull() const {
            return ptr == nullptr;
        }
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNEDCONTINUOUSMEMORYBLOCK_H
