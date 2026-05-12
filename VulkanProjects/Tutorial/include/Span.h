//
// Created by 沖田大河 on 2026/02/21.
//

#ifndef TUTORIAL_SPAN_H
#define TUTORIAL_SPAN_H

#include "SpanAllocator.h"
#include "SpanView.h"

#include <cstdint>
#include <limits>
#include <new>
#include <stdexcept>
#include <type_traits>

/**
 * 特定の型に限定された連続したメモリ領域を表す構造体
 * @tparam T アドレスの実体の型
 */
template<typename T>
struct Span {

    using StorageType = std::remove_const_t<T>;

    static Span createEmpty() { return Span(nullptr, 0); }

    [[nodiscard]] T* getHeadPtr() const { return reinterpret_cast<T*>(_headPtr); }

    [[nodiscard]] uint32_t getMaxElementCount() const { return _maxElementCount; }

    [[nodiscard]] uint32_t getElementCount() const { return emptyIndex; }

    [[nodiscard]] std::size_t getAllocatedBytes() const { return _memoryBlock.size.value(); }

    const T& operator [](const uint32_t index) const {
        const auto pointer = pointerAt(index);
        if (pointer == nullptr) {
            throw std::runtime_error("Span: element at index is not initialized");
        }
        return *pointer;
    }

    const T* pointerAt(const uint32_t index) const {
        if (_headPtr == nullptr) {
            throw std::runtime_error("Span: headPtr is deleted or not initialized");
        }
        if (index >= emptyIndex) {
            throw std::out_of_range("Span: index out of range");
        }
        auto elementPtr = _headPtr + index;
        return reinterpret_cast<T*>(elementPtr);
    }

    void Add(const T& content) {
        if (_maxElementCount == 0) {
            throw std::runtime_error("Span: cannot add element to a span with maxElementCount of 0");
        }
        if (emptyIndex >= _maxElementCount) {
            throw std::runtime_error("Span: cannot add element to a span that is already full");
        }
        auto elementPtr = _headPtr + emptyIndex;
        new (elementPtr) StorageType(content);
        ++emptyIndex;
    }

    void markFilled(uint32_t count) {
        if (count > _maxElementCount) {
            throw std::out_of_range("Span::markFilled: count exceeds maxElementCount");
        }
        emptyIndex = count;
    }

    void markFilled() {
        emptyIndex = _maxElementCount;
    }

    StorageType* mutablePointerAt(const uint32_t index) {
        if (index >= _maxElementCount) {
            throw std::out_of_range("Span: index out of range");
        }
        return _headPtr + index;
    }

    T* begin() {
        return reinterpret_cast<T*>(_headPtr);
    }

    const T* begin() const {
        return reinterpret_cast<T*>(_headPtr);
    }

    T* end() {
        return reinterpret_cast<T*>(_headPtr + emptyIndex);
    }

    const T* end() const {
        return reinterpret_cast<T*>(_headPtr + emptyIndex);
    }

    static Span stackAlloc(const uint32_t count) {
        if (count == 0) {
            throw std::runtime_error("Span: count must be greater than 0");
        }
        if (count > std::numeric_limits<std::size_t>::max() / sizeof(StorageType)) {
            throw std::overflow_error("Span: requested byte size overflow");
        }

        const auto block = SpanAllocator::getAllocator()->allocate(
            Tutorial::ResourceManagement::Memory::Bytes::fromSizeT(sizeof(StorageType) * static_cast<std::size_t>(count)),
            Tutorial::ResourceManagement::Memory::Alignment(Tutorial::ResourceManagement::Memory::Bytes::fromSizeT(alignof(StorageType)))
        );
        return Span(block, count);
    }

    void deallocAllWhenNonMoved() {
        if (_headPtr == nullptr) {
            return;
        }
        for (uint32_t i = 0; i < emptyIndex; ++i) {
            auto* elementPtr = _headPtr + (emptyIndex - 1 - i);
            elementPtr->~StorageType();
        }

        SpanAllocator::getAllocator()->deallocate(_memoryBlock);

        _headPtr = nullptr;
        _maxElementCount = 0;
        _memoryBlock = emptyMemoryBlock();
        emptyIndex = 0;
    }

    Span& operator=(const Span& other) = delete;
    Span(const Span& other) = delete;

    Span& operator=(Span&& other) noexcept {
        if (this != &other) {
            deallocAllWhenNonMoved();
            _headPtr = other._headPtr;
            _maxElementCount = other._maxElementCount;
            emptyIndex = other.emptyIndex;
            _memoryBlock = other._memoryBlock;

            other._headPtr = nullptr;
            other._maxElementCount = 0;
            other.emptyIndex = 0;
            other._memoryBlock = emptyMemoryBlock();
        }
        return *this;
    }

    SpanView<T> asView() const {
        return SpanView<T>(getHeadPtr(), getElementCount());
    }

    Span(Span&& other) noexcept
        : _headPtr(other._headPtr),
          _maxElementCount(other._maxElementCount),
          emptyIndex(other.emptyIndex),
          _memoryBlock(other._memoryBlock) {
        other._headPtr = nullptr;
        other._maxElementCount = 0;
        other.emptyIndex = 0;
        other._memoryBlock = emptyMemoryBlock();
    }

    ~Span() {
        deallocAllWhenNonMoved();
    }

private:
    StorageType* _headPtr;

    uint32_t _maxElementCount;

    uint32_t emptyIndex = 0;

    Tutorial::ResourceManagement::Memory::AlignedContinuousMemoryBlock _memoryBlock;

    explicit Span(Tutorial::ResourceManagement::Memory::AlignedContinuousMemoryBlock memoryBlock, const uint32_t count):
        _headPtr(static_cast<StorageType*>(memoryBlock.ptr)),
        _maxElementCount(count),
        _memoryBlock(memoryBlock) {
    }

    explicit Span(StorageType* headPtr, const uint32_t maxElementCount):
        _headPtr(headPtr),
        _maxElementCount(maxElementCount),
        _memoryBlock(emptyMemoryBlock()) {
    }

    static Tutorial::ResourceManagement::Memory::AlignedContinuousMemoryBlock emptyMemoryBlock() {
        return Tutorial::ResourceManagement::Memory::AlignedContinuousMemoryBlock{
            nullptr,
            Tutorial::ResourceManagement::Memory::Bytes::fromSizeT(0),
            Tutorial::ResourceManagement::Memory::Alignment(Tutorial::ResourceManagement::Memory::Bytes::fromSizeT(1))
        };
    }
};

#endif //TUTORIAL_SPAN_H
