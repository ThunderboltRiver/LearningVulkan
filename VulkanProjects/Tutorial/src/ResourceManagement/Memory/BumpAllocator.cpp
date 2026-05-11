

#include "ResourceManagement/Memory/BumpAlloc/BumpAllocator.h"

#include "ResourceManagement/Memory/OsMemory.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement::BumpAlloc {

    BumpAllocator::BumpAllocator(const Bytes arenaSize)
        : _arenaSize(arenaSize), _arenaHead(nullptr) {
        if (!arenaSize.isPowerOfTwo()) {
            throw std::invalid_argument("BumpAllocator: arena size must be a power of two");
        }
    }

    BumpAlloc::AlignedArena* BumpAllocator::allocateArena(const Alignment alignment) {
        auto* arena = new BumpAlloc::AlignedArena{
            allocateAlignedMemoryBlock(_arenaSize, alignment),
            _arenaHead
        };
        _arenaHead = arena;
        return arena;
    }

    Bytes BumpAllocator::getArenaSize() const {
        return _arenaSize;
    }

    BumpAllocator::~BumpAllocator() {
        auto* current = _arenaHead;
        while (current != nullptr) {
            auto* next = current->next;
            deallocateAlignedMemoryBlock(current->block);
            delete current;
            current = next;
        }
        _arenaHead = nullptr;
    }
}
