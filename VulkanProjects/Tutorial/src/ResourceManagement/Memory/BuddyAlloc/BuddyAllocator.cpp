#include "ResourceManagement/Memory/BuddyAlloc/BuddyAllocator.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {
    BuddyAllocator::BuddyAllocator(const Bytes arenaSize)
        : _bumpAllocator(arenaSize),
          _alignedAllocators(nullptr) {
        if (!arenaSize.isPowerOfTwo()) {
            throw std::invalid_argument("BuddyAllocator: arena size must be a power of two");
        }
    }

    AlignedBuddyAllocator* BuddyAllocator::getOrCreateAlignedAllocator(const Alignment alignment) {
        if (auto* existing = findAlignedAllocator(alignment); existing != nullptr) {
            return existing;
        }
        auto* created = new AlignedBuddyAllocator(alignment, _bumpAllocator);
        created->setNext(_alignedAllocators);
        _alignedAllocators = created;
        return created;
    }

    AlignedBuddyAllocator* BuddyAllocator::findAlignedAllocator(const Alignment alignment) const {
        auto* current = _alignedAllocators;
        while (current != nullptr) {
            if (current->alignment == alignment) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    AlignedContinuousMemoryBlock BuddyAllocator::allocate(const Bytes size, const Alignment alignment) {
        if (size.isZero()) {
            throw std::invalid_argument("BuddyAllocator: size must be greater than 0");
        }

        // まず要求alignmentそのものに対応するAlignedBuddyAllocatorを必ず用意する。
        // これにより、大きいalignmentのallocatorが先に存在していても、小さいalignment専用のアリーナを持てる。
        auto* requestedAllocator = getOrCreateAlignedAllocator(alignment);

        // 既存のAlignedBuddyAllocatorのうち、要求alignmentを満たせるものへ順に割り当てを試みる。
        // 例: 64バイトalignmentのallocatorは、16バイトalignment要求も満たせる。
        for (auto* allocator = _alignedAllocators; allocator != nullptr; allocator = allocator->next) {
            if (!allocator->satisfies(alignment)) {
                continue;
            }
            auto block = allocator->tryAllocate(size);
            if (!block.isNull()) {
                return block;
            }
        }

        // 既存allocatorでは割り当てできなかった場合だけ、要求alignment専用のallocatorへアリーナを追加する。
        return requestedAllocator->allocateWithNewArena(size, _bumpAllocator);
    }

    void BuddyAllocator::deallocate(const AlignedContinuousMemoryBlock block) {
        if (block.ptr == nullptr) {
            throw std::invalid_argument("BuddyAllocator: block ptr must not be null");
        }

        // allocate時に実際に使用したalignmentはblockに保存される。
        // 返却では新しいallocatorを作らず、既存の同一alignmentのallocatorへ処理を委譲する。
        auto* allocator = findAlignedAllocator(block.alignment);
        if (allocator == nullptr) {
            throw std::invalid_argument("BuddyAllocator: block does not belong to this allocator");
        }

        allocator->deallocate(block);
    }

    Bytes BuddyAllocator::getArenaSize() const {
        return _bumpAllocator.getArenaSize();
    }

    void BuddyAllocator::destroyMetadata() noexcept {
        auto* aligned = _alignedAllocators;
        while (aligned != nullptr) {
            auto* nextAligned = aligned->next;
            delete aligned;
            aligned = nextAligned;
        }
        _alignedAllocators = nullptr;
    }

    BuddyAllocator::~BuddyAllocator() {
        destroyMetadata();
    }
}
