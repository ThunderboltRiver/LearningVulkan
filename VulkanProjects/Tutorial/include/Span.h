//
// Created by 沖田大河 on 2026/02/21.
//

#ifndef TUTORIAL_SPAN_H
#define TUTORIAL_SPAN_H

#include "PlacementStackAllocator.h"
#include "SpanAllocator.h"

/**
 * 特定の型に限定された連続したメモリ領域を表す構造体
 * @tparam T アドレスの実体の型
 */
template<typename T>
struct Span {

    using StorageType = std::remove_const_t<T>;

    /**
     * 先頭のメモリアドレスへのポインタを取得する
     * @return 先頭のメモリアドレスへのポインタ
     */
    [[nodiscard]] T* getHeadPtr() const { return reinterpret_cast<T*>(_headPtr); }

    /**
     * この範囲に格納できる最大要素数を取得する
     * @return この範囲に格納できる最大要素数
     */
    [[nodiscard]] uint32_t getMaxElementCount() const { return _maxElementCount; }

    /**
     * 指定されたインデックスが指す実体への参照を返す
     * @param index インデックス
     * @return 指定されたインデックスが指す実体への参照
     */
    const T& operator [](const uint32_t index) const {
        const auto pointer = pointerAt(index);
        if (pointer == nullptr) {
            throw std::runtime_error("Span: element at index is not initialized");
        }
        return *pointer;
    }

    /**
     * 指定されたインデックスが指す実体へのポインタを返す
     * @param index インデックス
     * @return 指定されたインデックスが指す実体へのポインタ
     */
    const T* pointerAt(const uint32_t index) const {
        if (index >= _maxElementCount) {
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

    T* begin() const {
        return reinterpret_cast<T*>(_headPtr);
    }

    T* end() const {
        return reinterpret_cast<T*>(_headPtr + _maxElementCount);
    }

    /**
     * スタックアロケータを使用して、指定された要素数のSpanを割り当てる
     * 各要素は未初期化の状態で割り当てられる。割り当てられた領域に要素を配置する際は、placement newなどを使用して明示的に要素を初期化する必要がある
     * @param count 要素数
     * @return 割り当てられたSpan
     */
    static Span stackAlloc(const uint32_t count) {
        const auto allocResult = SpanAllocator::getAllocator()->stackAlloc<StorageType>(count);
        return Span(allocResult);
    }

    // コピー禁止。割り当てられた領域の所有権を持つのは一つのインスタンスのみにするため
    Span& operator=(const Span& other) = delete;
    Span(const Span& other) = delete;

    Span& operator=(Span&& other) noexcept {
        if (this != &other) {
            // 既にこのインスタンスが所有している領域を解放する
            this->~Span();

            // ムーブ元からムーブ先に所有権を移動する
            _headPtr = other._headPtr;
            _maxElementCount = other._maxElementCount;
            _allocatedBytes = other._allocatedBytes;

            // ムーブ元を空にする。これにより、ムーブ元のデストラクタは何も解放しない
            other._headPtr = nullptr;
            other._maxElementCount = 0;
            other._allocatedBytes = 0;
        }
        return *this;
    }

    /**
     * ムーブコンストラクタ。所有権をムーブ元からムーブ先に移動する。
     * ムーブ元は空の状態になり、デストラクタでは何もしない。
     */
    Span(Span&& other) noexcept
        : _headPtr(other._headPtr),
          _maxElementCount(other._maxElementCount),
          _allocatedBytes(other._allocatedBytes) {
        // ムーブ元を空にする。これにより、ムーブ元のデストラクタは何も解放しない
        other._headPtr = nullptr;
        other._maxElementCount = 0;
        other._allocatedBytes = 0;
    }

    ~Span() {
        // ムーブ済み（空）の場合は何もしない
        if (_headPtr == nullptr) {
            return;
        }
        // このSpanが所有する領域に格納されている要素をすべて破棄してから、スタックアロケータに領域を解放する
        // 無効なポインタの場合はデストラクタで例外がスローされ、リークする可能性があるが、それはこのクラスを使用する側の責任とする
        for (uint32_t i = 0; i < _maxElementCount; ++i) {
            auto elementPtr = pointerAt(_maxElementCount - 1 - i);
            elementPtr->~T();
        }
        SpanAllocator::getAllocator()->dealloc(_allocatedBytes);
    }

private:

    /**
     * 先頭のメモリアドレスへのポインタ
     */
    StorageType* _headPtr;

    /**
     * この範囲に格納できる最大要素数
     */
    uint32_t _maxElementCount;

    /**
     * まだ要素が配置されていないインデックス。Addなどで要素を配置する際に使用する
     */
    uint32_t emptyIndex = 0;

    /**
     * スタックアロケータから割り当てられた領域のバイト数。解放の際に必要
     */
    size_t _allocatedBytes;

    // stackAlloc経由でしかインスタンス化できないようにするため、コンストラクタはprivateにする
    explicit Span(PlacementStackAllocator::AllocResult<StorageType> allocResult):
        _headPtr(allocResult.headPtr),
        _maxElementCount(allocResult.count),
        _allocatedBytes(allocResult.allocatedBytes) {
    }
};

#endif //TUTORIAL_SPAN_H
