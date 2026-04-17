//
// Created by 沖田大河 on 2026/04/18.
//

#ifndef TUTORIAL_IREQUIREDVULKANINSTANCELAYERPROVIDER_H
#define TUTORIAL_IREQUIREDVULKANINSTANCELAYERPROVIDER_H

#include <cstdint>
#include "Span.h"

namespace Tutorial::Graphics {

    /**
     * 必須のVulkanインスタンスレイヤー一覧のプロバイダ
     */
    class RequiredVulkanInstanceLayerProvider {

#ifndef NDEBUG
        constexpr static bool enableValidationLayers = true;
#else
        constexpr static bool enableValidationLayers = false;
#endif

    public:
        
        /**
         * サポートされる必要のあるVulkanインスタンスレイヤーの名称一覧を取得する
         * @return サポートされる必要のあるVulkanインスタンスレイヤーの名称一覧を格納するためのchar const*型の配列へのSpan
         */
        Span<char const*> getLayerNames() const;
    };
}


#endif //TUTORIAL_IREQUIREDVULKANINSTANCELAYERPROVIDER_H