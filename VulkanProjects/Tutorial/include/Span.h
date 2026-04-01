//
// Created by 沖田大河 on 2026/02/21.
//

#ifndef TUTORIAL_SPAN_H
#define TUTORIAL_SPAN_H

#include "PlacementStackAllocator.h"
#include "SpanAllocator.h"
#include "type_traits"

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

    /**
     * 外部（Vulkan APIなど）がgetHeadPtr()経由でバッファに直接書き込んだ後に呼び出す。
     * emptyIndexを指定された要素数に設定する。
     * @param count 書き込まれた要素数。省略した場合はmaxElementCount（全要素が書き込まれたとみなす）
     */
    void markFilled(uint32_t count) {
        if (count > _maxElementCount) {
            throw std::out_of_range("Span::markFilled: count exceeds maxElementCount");
        }
        emptyIndex = count;
    }

    void markFilled() {
        emptyIndex = _maxElementCount;
    }

    /**
     * 指定されたインデックスが指す実体への書き込み可能なポインタを返す。
     * Addを使わず、直接要素を初期化する場合に使用する。
     * emptyIndexは更新されないため、呼び出し後にmarkFilledを呼ぶこと。
     * @param index インデックス
     * @return 指定されたインデックスが指す実体への書き込み可能なポインタ
     */
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

    /**
     * ムーブされていない場合に、割り当てられた領域をすべて破棄してスタックアロケータに返す。
     * ムーブされた（空の）場合は何もしない。これにより、ムーブ元のデストラクタは何も解放しない。
     */
    void deallocAllWhenNonMoved() {
        // ムーブ済み（空）の場合は何もしない
        if (_headPtr == nullptr) {
            return;
        }
        // 初期化された要素をすべて破棄
        for (uint32_t i = 0; i < emptyIndex; ++i) {
            auto elementPtr = pointerAt(emptyIndex - 1 - i);
            elementPtr->~T();
        }

        // 割り当てられた領域をスタックアロケータに返す
        SpanAllocator::getAllocator()->dealloc(_allocatedBytes);

        // このインスタンスを空にする。これにより、デストラクタで二重解放が起こるのを防ぐ
        _headPtr = nullptr;
        _maxElementCount = 0;
        _allocatedBytes = 0;
        emptyIndex = 0;
    }

    // コピー禁止。割り当てられた領域の所有権を持つのは一つのインスタンスのみにするため
    Span& operator=(const Span& other) = delete;
    Span(const Span& other) = delete;

    Span& operator=(Span&& other) noexcept {
        // SpanはStackAlloc経由でインスタンス化されるため、自己代入に加えて
        // ムーブ元がムーブ先よりも先にstackAllocされていることをチェックする必要がある
        // 例えば、span1 = span2のようなコードがあった場合、span2はspan1よりも先にstackAllocされている必要がある
        // そうでない場合、span1のストレージの解放がspan2のストレージの解放よりも先に行われてしまい、span2のデストラクタで無効なポインタを解放しようとして例外がスローされる可能性がある
        if (this != &other && other._headPtr < _headPtr) {
            this->deallocAllWhenNonMoved();
        }
        _headPtr = other._headPtr;
        _maxElementCount = other._maxElementCount;
        _allocatedBytes = other._allocatedBytes;
        emptyIndex = other.emptyIndex;
        return *this;
    }

    /**
     * ムーブコンストラクタ。所有権をムーブ元からムーブ先に移動する。
     * ムーブ元は空の状態になり、デストラクタでは何もしない。
     */
    Span(Span&& other) noexcept
        : _headPtr(other._headPtr),
          _maxElementCount(other._maxElementCount),
          emptyIndex(other.emptyIndex),
          _allocatedBytes(other._allocatedBytes) {
        // ムーブ元を空にする。これにより、ムーブ元のデストラクタは何も解放しない
        other._headPtr = nullptr;
        other._maxElementCount = 0;
        other._allocatedBytes = 0;
        other.emptyIndex = 0;
    }

    ~Span() {
        deallocAllWhenNonMoved();
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
