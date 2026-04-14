//
// Created by 沖田大河 on 2026/04/12.
//

#ifndef TUTORIAL_SWAP_CHAIN_IMAGE_COUNT_SELECT_STRATEGY_H
#define TUTORIAL_SWAP_CHAIN_IMAGE_COUNT_SELECT_STRATEGY_H

#include "vulkan/vulkan.h"

namespace Tutorial::Graphics {
    class SwapChainImageCountSelectStrategy {
        constexpr static uint32_t DEFAULT_IMAGE_COUNT = 3;
        public:

        explicit SwapChainImageCountSelectStrategy();

        /**
         * サーフェイスの能力から、SwapChainの画像の枚数を選択する
         * @param surfaceCapabilities サーフェイスの能力
         * @return SwapChainの画像の枚数
         */
        [[nodiscard]] uint32_t chooseImageCount(const VkSurfaceCapabilitiesKHR &surfaceCapabilities) const;

    };
};

#endif //TUTORIAL_SWAP_CHAIN_IMAGE_COUNT_SELECT_STRATEGY_H