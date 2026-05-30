#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNEDCONTINUOUSMEMORYBLOCK_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNEDCONTINUOUSMEMORYBLOCK_H

#include "ResourceManagement/Memory/Alignment.h"

namespace Tutorial::ResourceManagement::Memory {

    /**
     * 仮想アドレス空間上で連続した、指定アライメント済みのメモリブロック。
     * ptr は先頭アドレス、size は利用可能な領域サイズ、alignment は ptr が満たすアライメントを表す。
     */
    struct AlignedContinuousMemoryBlock {
        void* ptr;
        Bytes size;
        Alignment alignment;

        [[nodiscard]] static AlignedContinuousMemoryBlock empty(Alignment alignment);

        [[nodiscard]] bool isNull() const;

        [[nodiscard]] bool contains(const void* targetPtr) const;
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_ALIGNEDCONTINUOUSMEMORYBLOCK_H
