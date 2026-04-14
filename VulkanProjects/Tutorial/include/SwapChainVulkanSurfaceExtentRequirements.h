//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_EXTENT_REQUIREMENTS_H
#define TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_EXTENT_REQUIREMENTS_H

#include "FrameBufferSize.h"
#include "SwapChainVulkanSurfaceFormatRequirements.h"

namespace Tutorial::Graphics {
    class SwapChainVulkanSurfaceExtentRequirements {
        const FrameBufferSize _frameBufferSize;

        uint32_t clampToRange(uint32_t value, uint32_t min, uint32_t max) const;

    public:
        explicit SwapChainVulkanSurfaceExtentRequirements(FrameBufferSize frameBufferSize);

        /**
         * 要求を満たすエクステントを選択する
         * @param surfaceCapabilities サーフェイスの能力
         * @return 要求を満たすExtent2D
         */
        [[nodiscard]] VkExtent2D chooseSatisfiedOne(VkSurfaceCapabilitiesKHR surfaceCapabilities) const;
    };
};


#endif //TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_EXTENT_REQUIREMENTS_H