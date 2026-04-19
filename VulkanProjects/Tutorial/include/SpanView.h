//
// Created by 沖田大河 on 2026/04/19.
//

#ifndef TUTORIAL_SPANVIEW_H
#define TUTORIAL_SPANVIEW_H

#include <cstdint>
#include <stdexcept>


/**
 *　特定の型に限定された連続したメモリ領域を表す構造体
 * SpanViewはSpanが指している領域への所有権を持たない読み取り専用のビューである。
 * SpanViewはSpanから生成されることを想定している
 * @tparam T 配列の要素の型
 */
template<typename T>
class SpanView {
    const T* _headPtr;
    const uint32_t _elementCount;

public:
    explicit SpanView(const T* headPtr, const uint32_t elementCount):
        _headPtr(headPtr),
        _elementCount(elementCount) {
    }

    [[nodiscard]] const T* getHeadPtr() const { return _headPtr; }

    [[nodiscard]] uint32_t getElementCount() const { return _elementCount; }

    const T* begin() const { return _headPtr; }

    const T* end() const { return _headPtr + _elementCount; }

    const T& operator [](const uint32_t index) const {
        if (index >= _elementCount) {
            throw std::runtime_error("SpanView: index out of bounds");
        }
        return _headPtr[index];
    }

    SpanView(const SpanView& other) = default;
    SpanView(SpanView&& other) noexcept = default;

    SpanView& operator=(const SpanView& other) = default;
    SpanView& operator=(SpanView&& other) noexcept = default;

    ~SpanView() = default;
};


#endif //TUTORIAL_SPANVIEW_H