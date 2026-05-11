#include "ResourceManagement/Memory/OsMemory.h"

#include <cstring>
#include <new>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace Tutorial::ResourceManagement {
    namespace {
        struct AllocationHeader {
            void* mappedPtr;
            Bytes mappedSize;
        };

        Alignment effectiveAlignment(const Alignment alignment) {
            return Alignment(alignment.bytes().max(Bytes::fromSizeT(alignof(AllocationHeader))));
        }

        Bytes mappedBytesFor(const Bytes size, const Alignment alignment) {
            if (size.isZero()) {
                throw std::invalid_argument("OsMemory: size must be greater than 0");
            }
            const auto headerBytes = Bytes::fromSizeT(sizeof(AllocationHeader));
            return size + alignment.bytes() + headerBytes;
        }
    }

    AlignedContinuousMemoryBlock allocateAlignedMemoryBlock(const Bytes size, const Alignment alignment) {
        const Alignment effective = effectiveAlignment(alignment);
        const Bytes mappedBytes = mappedBytesFor(size, effective);

#ifdef _WIN32
        void* mappedPtr = VirtualAlloc(nullptr, mappedBytes.value(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (mappedPtr == nullptr) {
            throw std::bad_alloc();
        }
#else
        void* mappedPtr = mmap(nullptr, mappedBytes.value(), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (mappedPtr == MAP_FAILED) {
            throw std::bad_alloc();
        }
#endif

        const auto rawAddress = reinterpret_cast<std::uintptr_t>(mappedPtr);
        const auto alignedAddress = effective.alignUp(rawAddress + sizeof(AllocationHeader));
        auto* header = reinterpret_cast<AllocationHeader*>(alignedAddress - sizeof(AllocationHeader));
        header->mappedPtr = mappedPtr;
        header->mappedSize = mappedBytes;

        return AlignedContinuousMemoryBlock{
            reinterpret_cast<void*>(alignedAddress),
            size,
            effective
        };
    }

    void deallocateAlignedMemoryBlock(const AlignedContinuousMemoryBlock block) {
        if (block.ptr == nullptr) {
            throw std::invalid_argument("OsMemory: block ptr must not be null");
        }
        auto* header = reinterpret_cast<AllocationHeader*>(
            reinterpret_cast<std::uintptr_t>(block.ptr) - sizeof(AllocationHeader)
        );
#ifdef _WIN32
        if (VirtualFree(header->mappedPtr, 0, MEM_RELEASE) == 0) {
            throw std::runtime_error("OsMemory: VirtualFree failed");
        }
#else
        if (munmap(header->mappedPtr, header->mappedSize.value()) != 0) {
            throw std::runtime_error("OsMemory: munmap failed");
        }
#endif
    }
}
