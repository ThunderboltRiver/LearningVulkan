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

    /**
     * 先頭のメモリアドレスへのポインタ
     */
    T* const headPtr;

    /**
     * この範囲に格納できる最大要素数
     */
    const uint32_t maxElementCount;

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
    T* pointerAt(const uint32_t index) const {
        if (index >= maxElementCount) {
            throw std::out_of_range("Span: index out of range");
        }
        auto elementPtr = headPtr + index;
        return elementPtr;
    }

    T* begin() const {
        return headPtr;
    }

    T* end() const {
        return headPtr + maxElementCount;
    }

    /**
     * スタックアロケータを使用して、指定された要素数のSpanを割り当てる
     * 各要素は未初期化の状態で割り当てられる。割り当てられた領域に要素を配置する際は、placement newなどを使用して明示的に要素を初期化する必要がある
     * @param count 要素数
     * @return 割り当てられたSpan
     */
    static Span stackAlloc(const uint32_t count) {
        const auto allocResult = SpanAllocator::getAllocator()->stackAlloc<T>(count);
        return Span(allocResult);
    }

    // コピー禁止。割り当てられた領域の所有権を持つのは一つのインスタンスのみにするため
    Span& operator=(const Span& other) = delete;
    Span(const Span& other) = delete;

    ~Span() {
        // このSpanが所有する領域に格納されている要素をすべて破棄してから、スタックアロケータに領域を解放する
        // 無効なポインタの場合はデストラクタで例外がスローされ、リークする可能性があるが、それはこのクラスを使用する側の責任とする
        for (uint32_t i = 0; i < maxElementCount; ++i) {
            auto elementPtr = pointerAt(maxElementCount - 1 - i);
            elementPtr->~T();
        }
        SpanAllocator::getAllocator()->dealloc(_allocatedBytes);
    }

private:

    /**
     * スタックアロケータから割り当てられた領域のバイト数。解放の際に必要
     */
    const size_t _allocatedBytes;

    // stackAlloc経由でしかインスタンス化できないようにするため、コンストラクタはprivateにする
    explicit Span(PlacementStackAllocator::AllocResult<T> allocResult):
        headPtr(allocResult.headPtr),
        maxElementCount(allocResult.count),
        _allocatedBytes(allocResult.allocatedBytes) {
    }
};

#endif //TUTORIAL_SPAN_H