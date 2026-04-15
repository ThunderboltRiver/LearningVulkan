//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_EXTENT_PREFERENCE_H
#define TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_EXTENT_PREFERENCE_H

#include <vulkan/vulkan.h>
#include "Graphics/FrameBufferSize.h"

namespace Tutorial::Graphics {

    /**
     * SwapChainの作成に使用されるサーフェイスに対する選好を表すクラス
     */
    class SwapChainVulkanSurfaceExtentPreference {
        const FrameBufferSize _frameBufferSize;

        uint32_t clampToRange(uint32_t value, uint32_t min, uint32_t max) const;

    public:
        explicit SwapChainVulkanSurfaceExtentPreference(FrameBufferSize frameBufferSize);

        /**
         * この選好を満たすエクステントを選択する
         * @param surfaceCapabilities サーフェイスの能力
         * @return 選好を満たすExtent2D
         */
        [[nodiscard]] VkExtent2D chooseSatisfiedOne(VkSurfaceCapabilitiesKHR surfaceCapabilities) const;
    };
};


#endif //TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_EXTENT_PREFERENCE_H