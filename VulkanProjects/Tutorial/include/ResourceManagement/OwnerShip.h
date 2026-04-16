//
// Created by 沖田大河 on 2026/04/16.
//

#ifndef TUTORIAL_OWNERSHIP_H
#define TUTORIAL_OWNERSHIP_H

#include "Borrowed.h"

namespace Tutorial::ResourceManagement {

    /**
     * リソースへの所有権を表すためのマーカークラス
     * TPtrは所有権の対象となるリソースへのポインタ型
     */
    template<ResourcePtr TResourcePtr>
    class OwnerShip {
        TResourcePtr _pTargetResource;
    public:

        explicit OwnerShip(TResourcePtr pTargetResource): _pTargetResource(pTargetResource) {
        }

        /**
         * 所有権が移動されたことを表す特別なOwnerShipオブジェクト
         */
        static OwnerShip MOVED() { return OwnerShip(nullptr); }

        /**
         * リソースへのハンドラを返す
         * @return リソースへの生のポインタを返す
         */
        TResourcePtr getRawHandle() const { return _pTargetResource; }

        /**
         *　リソースへの借用を表すBorrowedオブジェクトを返す
         * @return リソースへの借用
         */
        Borrowed<TResourcePtr> borrow() const { return Borrowed<TResourcePtr>(_pTargetResource); }

        /**
         * 所有権を移動させるためのメソッド。
         * 呼び出すと、このインスタンスが指すポインタがnullptrに設定され、リソースへの所有権が新しいOwnerShipオブジェクトに移動する。
         * @return リソースへの所有権を持つ新しいOwnerShipオブジェクト
         */
        OwnerShip move() {
            OwnerShip newOwner(_pTargetResource);
            _pTargetResource = nullptr; // 所有権が移動したことを表すためにムーブ元のリソースへのポインタをnullptrにする
            return newOwner;
        }

        // コピー禁止
        OwnerShip(const OwnerShip& other) = delete;

        // コピー代入禁止
        OwnerShip& operator=(const OwnerShip& other) = delete;

        // ムーブコンストラクタ
        OwnerShip(OwnerShip&& other) noexcept {
            _pTargetResource = other._pTargetResource;
            other._pTargetResource = nullptr; // ムーブ元のリソースへのポインタをnullptrにして所有権が移動したことを表す
        };

        // ムーブ代入演算子
        OwnerShip& operator=(OwnerShip&& other) noexcept {;
            if (this != &other) {
                _pTargetResource = other._pTargetResource;
                other._pTargetResource = nullptr; // ムーブ元のリソースへのポインタをnullptrにして所有権が移動したことを表す
            }
            return *this;
        }

        // 等価演算子
        bool operator==(const OwnerShip& other) const {
            return _pTargetResource == other._pTargetResource;
        }

        // 非等価演算子
        bool operator!=(const OwnerShip& other) const {
            return !(*this == other);
        }

        [[nodiscard]] bool isNotMoved() const { return _pTargetResource != nullptr; }

        // デストラクタ
        ~OwnerShip() = default;
    };
}

#endif //TUTORIAL_OWNERSHIP_H