#include "ResourceManagement/Memory/ContinuousMemoryBlockPool.h"
#include "ResourceManagement/Memory/LargeSizeAllocator.h"
#include "Span.h"
#include "SpanAllocator.h"

#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

namespace {
    using Tutorial::ResourceManagement::BuddyAllocator;
    using Tutorial::ResourceManagement::Alignment;
    using Tutorial::ResourceManagement::Bytes;
    using Tutorial::ResourceManagement::ContinuousMemoryBlockPool;
    using Tutorial::ResourceManagement::LargeSizeAllocator;

    void require(const bool condition, const char* message) {
        if (!condition) {
            throw std::runtime_error(message);
        }
    }

    template<typename Exception, typename Func>
    void requireThrows(Func&& func, const char* message) {
        try {
            func();
        } catch (const Exception&) {
            return;
        }
        throw std::runtime_error(message);
    }

    bool isAligned(void* ptr, const Alignment alignment) {
        return reinterpret_cast<std::uintptr_t>(ptr) % alignment.bytes().value() == 0;
    }

    void testBytes() {
        const auto oneKiB = Bytes::fromKiB(1);
        const auto twoKiB = Bytes::fromSizeT(2048);
        require(oneKiB + oneKiB == twoKiB, "Bytes addition failed");
        require(twoKiB - oneKiB == oneKiB, "Bytes subtraction failed");
        require(oneKiB * 2 == twoKiB, "Bytes multiplication failed");
        require(twoKiB / 2 == oneKiB, "Bytes division failed");
        require(twoKiB / oneKiB == 2, "Bytes ratio division failed");
        require(twoKiB % oneKiB == Bytes::fromSizeT(0), "Bytes modulo failed");
        requireThrows<std::underflow_error>([] { (void)(Bytes::fromSizeT(1) - Bytes::fromSizeT(2)); }, "Bytes underflow was not detected");
        requireThrows<std::overflow_error>([] { (void)(Bytes::fromSizeT(std::numeric_limits<std::size_t>::max()) + Bytes::fromSizeT(1)); }, "Bytes overflow was not detected");
    }

    void testBytesPowerOfTwoHelpers() {
        require(Bytes::fromSizeT(1).isPowerOfTwo(), "1 must be a power of two");
        require(Bytes::fromSizeT(1024).isPowerOfTwo(), "1024 must be a power of two");
        require(!Bytes::fromSizeT(3).isPowerOfTwo(), "3 must not be a power of two");
        require(Bytes::fromSizeT(17).roundUpToPowerOfTwo() == Bytes::fromSizeT(32), "power-of-two round-up failed");
        require(Alignment(Bytes::fromSizeT(3)).bytes() == Bytes::fromSizeT(4), "Alignment did not round up to power of two");
        requireThrows<std::invalid_argument>([] { (void)Alignment(Bytes::fromSizeT(0)); }, "zero alignment was not rejected");
    }

    void testLargeSizeAllocator() {
        LargeSizeAllocator allocator;
        const auto block = allocator.allocate(Bytes::fromKiB(5), Alignment(Bytes::fromSizeT(64)));
        require(block.size == Bytes::fromKiB(8), "large allocation was not rounded to power of two");
        require(isAligned(block.ptr, Alignment(Bytes::fromSizeT(64))), "large allocation alignment failed");
        allocator.deallocate(block);
    }

    void testBuddyAllocator() {
        BuddyAllocator allocator(Bytes::fromKiB(4), Bytes::fromSizeT(16));
        auto first = allocator.allocate(Bytes::fromSizeT(32), Alignment(Bytes::fromSizeT(16)));
        auto second = allocator.allocate(Bytes::fromSizeT(32), Alignment(Bytes::fromSizeT(16)));
        require(first.ptr != second.ptr, "buddy allocator returned duplicate blocks");
        require(isAligned(first.ptr, Alignment(Bytes::fromSizeT(16))), "buddy allocation alignment failed");
        allocator.deallocate(first);
        allocator.deallocate(second);

        auto merged = allocator.allocate(Bytes::fromKiB(4), Alignment(Bytes::fromSizeT(16)));
        require(merged.size == Bytes::fromKiB(4), "buddy allocator did not return merged arena-sized block");
        allocator.deallocate(merged);
    }

    void testContinuousMemoryBlockPool() {
        ContinuousMemoryBlockPool pool(Bytes::fromKiB(4));
        auto small = pool.allocate(Bytes::fromSizeT(128), Alignment(Bytes::fromSizeT(32)));
        require(small.size <= Bytes::fromKiB(4), "small allocation should come from buddy allocator");
        require(isAligned(small.ptr, Alignment(Bytes::fromSizeT(32))), "small pool allocation alignment failed");
        pool.deallocate(small);

        auto large = pool.allocate(Bytes::fromKiB(8), Alignment(Bytes::fromSizeT(64)));
        require(large.size == Bytes::fromKiB(8), "large pool allocation should keep rounded large size");
        require(isAligned(large.ptr, Alignment(Bytes::fromSizeT(64))), "large pool allocation alignment failed");
        pool.deallocate(large);
    }

    void testSpanAllocation() {
        ContinuousMemoryBlockPool pool(Bytes::fromKiB(4));
        SpanAllocator::setAllocator(&pool);
        {
            auto span = Span<int>::stackAlloc(3);
            span.Add(10);
            span.Add(20);
            require(span.getElementCount() == 2, "Span Add did not update element count");
            require(span[0] == 10 && span[1] == 20, "Span values were not preserved");

            auto moved = Span<int>(std::move(span));
            require(moved.getElementCount() == 2, "Span move did not preserve element count");
            require(moved[0] == 10 && moved[1] == 20, "Span move did not preserve values");
        }
        {
            auto span = Span<int>::stackAlloc(2);
            *span.mutablePointerAt(0) = 1;
            *span.mutablePointerAt(1) = 2;
            span.markFilled();
            require(span.getElementCount() == 2, "Span markFilled did not update element count");
        }
        SpanAllocator::resetAllocator();
    }
}

int main() {
    try {
        testBytes();
        testBytesPowerOfTwoHelpers();
        testLargeSizeAllocator();
        testBuddyAllocator();
        testContinuousMemoryBlockPool();
        testSpanAllocation();
    } catch (const std::exception& exception) {
        std::cerr << "ResourceManagementSelfTest failed: " << exception.what() << '\n';
        return 1;
    }
    std::cout << "ResourceManagementSelfTest passed\n";
    return 0;
}
