#include "ResourceManagement/Memory/BuddyAlloc/BuddyAllocator.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {
    BuddyAllocator::BuddyAllocator(const Alignment minAlignment, const std::size_t alignmentCount, const Bytes arenaSize)
        : _bumpAllocator(arenaSize),
          _alignedAllocators(nullptr) {
        if (!arenaSize.isPowerOfTwo()) {
            throw std::invalid_argument("BuddyAllocator: arena size must be a power of two");
        }
        if (alignmentCount == 0) {
            throw std::invalid_argument("BuddyAllocator: alignment count must be greater than 0");
        }
        initializeAlignedAllocators(minAlignment, alignmentCount);
    }

    void BuddyAllocator::initializeAlignedAllocators(const Alignment minAlignment, const std::size_t alignmentCount) {
        Bytes alignmentBytes = minAlignment.bytes();
        for (std::size_t i = 1; i < alignmentCount; ++i) {
            alignmentBytes = alignmentBytes * 2;
        }
        for (std::size_t i = alignmentCount; i > 0; --i) {
            addAlignedAllocator(Alignment(alignmentBytes));
            if (i > 1) {
                alignmentBytes = alignmentBytes / 2;
            }
        }
    }

    void BuddyAllocator::addAlignedAllocator(const Alignment alignment) {
        auto* created = new AlignedBuddyAllocator(alignment, _bumpAllocator);
        created->setNext(_alignedAllocators);
        _alignedAllocators = created;
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

    AlignedBuddyAllocator* BuddyAllocator::findSatisfyingAlignedAllocator(
        const Alignment alignment
    ) const {
        for (auto* allocator = _alignedAllocators; allocator != nullptr; allocator = allocator->next) {
            if (allocator->alignment == alignment) {
                continue;
            }
            if (!allocator->satisfies(alignment)) {
                continue;
            }
            return allocator;
        }
        return nullptr;
    }

    AlignedContinuousMemoryBlock BuddyAllocator::allocate(const Bytes size, const Alignment alignment) {
        if (size.isZero()) {
            throw std::invalid_argument("BuddyAllocator: size must be greater than 0");
        }

        auto* requestedAllocator = findAlignedAllocator(alignment);
        if (requestedAllocator != nullptr) {
            auto block = requestedAllocator->tryAllocate(size);
            if (!block.isNull()) {
                return block;
            }
        }

        // 既存のAlignedBuddyAllocatorのうち、要求alignmentを満たせるものへ順に割り当てを試みる。
        // 例: 64バイトalignmentのallocatorは、16バイトalignment要求も満たせる。
        auto* satisfyingAllocator = findSatisfyingAlignedAllocator(alignment);
        if (satisfyingAllocator != nullptr) {
            auto block = satisfyingAllocator->tryAllocate(size);
            if (!block.isNull()) {
                return block;
            }
        }

        // 既存allocatorでは割り当てできなかった場合だけ、対応済みalignmentのallocatorへアリーナを追加する。
        if (requestedAllocator != nullptr) {
            return requestedAllocator->allocateWithNewArena(size, _bumpAllocator);
        }
        if (satisfyingAllocator != nullptr) {
            return satisfyingAllocator->allocateWithNewArena(size, _bumpAllocator);
        }
        throw std::invalid_argument("BuddyAllocator: unsupported alignment");
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
