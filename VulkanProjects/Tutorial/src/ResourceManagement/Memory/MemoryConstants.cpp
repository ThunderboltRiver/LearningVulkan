#include "ResourceManagement/Memory/MemoryConstants.h"

namespace Tutorial::ResourceManagement::Memory {

    const Bytes MIN_MEMORY_BLOCK_SIZE = Bytes::fromSizeT(sizeof(void*) * 2);

    const Bytes DEFAULT_ARENA_SIZE = Bytes::fromMiB(32);
}
