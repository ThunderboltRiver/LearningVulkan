#include "ResourceManagement/Memory/BuddyAlloc/BuddyAllocator.h"

#include "ResourceManagement/Memory/BuddyAlloc/BuddyOrderThreshold.h"

#include <stdexcept>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {
    BuddyAllocator::BuddyAllocator(const Bytes arenaSize)
        : _bumpAllocator(arenaSize),
          _alignedAllocators(nullptr),
          _minBlockSize(calculateMinBlockSize(arenaSize)),
          _maxOrder(calculateMaxOrder(arenaSize, _minBlockSize)) {
        if (!arenaSize.isPowerOfTwo()) {
            throw std::invalid_argument("BuddyAllocator: arena size must be a power of two");
        }
    }

    Bytes BuddyAllocator::calculateMinBlockSize(const Bytes arenaSize) {
        const std::size_t arenaExponent = arenaSize.log2PowerOfTwo();
        const std::size_t thresholdMinExponent = arenaExponent >= BUDDY_ORDER_THRESHOLD
            ? arenaExponent - BUDDY_ORDER_THRESHOLD
            : 0;
        const std::size_t freeBlockExponent = Bytes::fromSizeT(sizeof(FreeBlock)).roundUpToPowerOfTwo().log2PowerOfTwo();
        const std::size_t minBlockExponent = thresholdMinExponent >= freeBlockExponent
            ? thresholdMinExponent
            : freeBlockExponent;
        return Bytes::fromPowerOfTwoExponent(minBlockExponent);
    }

    BuddyOrder BuddyAllocator::calculateMaxOrder(const Bytes arenaSize, const Bytes minBlockSize) {
        if (minBlockSize > arenaSize) {
            throw std::invalid_argument("BuddyAllocator: min block size must not exceed arena size");
        }
        const std::size_t arenaExponent = arenaSize.log2PowerOfTwo();
        const std::size_t minBlockExponent = minBlockSize.log2PowerOfTwo();
        const std::size_t maxOrder = arenaExponent - minBlockExponent;
        if (maxOrder > BUDDY_ORDER_THRESHOLD) {
            throw std::invalid_argument("BuddyAllocator: too many buddy orders");
        }
        return BuddyOrder{static_cast<std::uint8_t>(maxOrder)};
    }

    AlignedBuddyAllocator* BuddyAllocator::getOrCreateAlignedAllocator(const Alignment alignment) {
        if (auto* existing = findAlignedAllocator(alignment); existing != nullptr) {
            return existing;
        }
        auto* created = new AlignedBuddyAllocator(alignment, _maxOrder, _minBlockSize, _bumpAllocator);
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

    BuddyOrder BuddyAllocator::orderFor(const Bytes size) const {
        const Bytes rounded = size.max(_minBlockSize).roundUpToPowerOfTwo();
        if (rounded > getArenaSize()) {
            throw std::invalid_argument("BuddyAllocator: requested size exceeds arena size");
        }
        BuddyOrder order{0};
        Bytes current = _minBlockSize;
        while (current < rounded) {
            current = current * 2;
            ++order.value;
        }
        return order;
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

            // allocator自身のalignmentを満たすブロックサイズにする必要がある。
            // 64バイトalignmentのallocatorで16バイトブロックを返すと、64バイト境界を保証できない。
            const BuddyOrder targetOrder = orderFor(size.max(allocator->alignment.bytes()));
            auto block = allocator->tryAllocate(size, targetOrder, _minBlockSize, _maxOrder);
            if (!block.isNull()) {
                return block;
            }
        }

        // 既存allocatorでは割り当てできなかった場合だけ、要求alignment専用のallocatorへアリーナを追加する。
        const BuddyOrder targetOrder = orderFor(size.max(alignment.bytes()));
        return requestedAllocator->allocateWithNewArena(size, targetOrder, _minBlockSize, _maxOrder, _bumpAllocator);
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

        const BuddyOrder order = orderFor(block.size.max(block.alignment.bytes()));
        allocator->deallocate(block, order, _minBlockSize, _maxOrder);
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
