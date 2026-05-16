#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_LARGESIZEALLOCATOR_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_LARGESIZEALLOCATOR_H

#include "ResourceManagement/Memory/MemoryBlock.h"

namespace Tutorial::ResourceManagement::Memory {

    /**
     * アリーナサイズを超える大きな連続メモリブロックをOSから直接確保するアロケータ。
     */
    class LargeSizeAllocator {
    public:
        /** size以上の最小2冪サイズへ丸め、alignmentを満たすブロックを確保する。 */
        [[nodiscard]] AlignedContinuousMemoryBlock allocate(Bytes size, Alignment alignment) const;

        /** allocateで確保したブロックをOSへ返却する。 */
        void deallocate(AlignedContinuousMemoryBlock block) const;
    };
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_LARGESIZEALLOCATOR_H
