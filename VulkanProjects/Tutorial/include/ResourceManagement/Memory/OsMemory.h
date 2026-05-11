#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_OSMEMORY_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_OSMEMORY_H

#include "ResourceManagement/Memory/MemoryBlock.h"

namespace Tutorial::ResourceManagement {

    /** OSから指定サイズ・指定アライメントの連続メモリブロックを確保する。 */
    [[nodiscard]] AlignedContinuousMemoryBlock allocateAlignedMemoryBlock(Bytes size, Alignment alignment);

    /** allocateAlignedMemoryBlockで確保したブロックをOSへ返却する。 */
    void deallocateAlignedMemoryBlock(AlignedContinuousMemoryBlock block);
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_OSMEMORY_H
