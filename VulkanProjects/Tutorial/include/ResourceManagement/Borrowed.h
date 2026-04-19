//
// Created by 沖田大河 on 2026/04/16.
//

#ifndef TUTORIAL_BORROWED_H
#define TUTORIAL_BORROWED_H
#include "ResourcePtrConcept.h"


namespace Tutorial::ResourceManagement
{
    /**
     * リソースへの借用を表すためのマーカークラス
     * TPtrは借用の対象となるリソースへのポインタ型
     */
    template <ResourcePtr TResourcePtr>
    class Borrowed {
        TResourcePtr _pTargetResource;

    public:
        static Borrowed Null() { return Borrowed(nullptr); }

        explicit Borrowed(TResourcePtr pTargetResource): _pTargetResource(pTargetResource) {
        }

        // リソースへのハンドラを返す
        TResourcePtr getRawHandle() const { return _pTargetResource; }

        // コピーコンストラクタ
        Borrowed(const Borrowed& other) = default;

        // コピー代入演算子
        Borrowed& operator=(const Borrowed& other) = default;

        // ムーブコンストラクタ
        Borrowed(Borrowed&& other) noexcept = default;

        // ムーブ代入演算子
        Borrowed& operator=(Borrowed&& other) noexcept = default;

        // 等価演算子
        bool operator==(const Borrowed& other) const {
            return _pTargetResource == other._pTargetResource;
        }

        // 非等価演算子
        bool operator!=(const Borrowed& other) const {
            return !(*this == other);
        }

        // デストラクタ
        ~Borrowed() = default;
    };
}


#endif //TUTORIAL_BORROWED_H