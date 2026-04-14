//
// Created by 沖田大河 on 2026/04/11.
//

#include "SwapChainVulkanSurfaceExtentRequirements.h"
#include <algorithm>

namespace Tutorial::Graphics {
    uint32_t SwapChainVulkanSurfaceExtentRequirements::clampToRange(uint32_t value, uint32_t min, uint32_t max) const {
        return std::max(min, std::min(max, value));
    }

    SwapChainVulkanSurfaceExtentRequirements::SwapChainVulkanSurfaceExtentRequirements(FrameBufferSize frameBufferSize):
        _frameBufferSize(frameBufferSize) {
    }

    VkExtent2D SwapChainVulkanSurfaceExtentRequirements::chooseSatisfiedOne(VkSurfaceCapabilitiesKHR surfaceCapabilities) const {
        if (surfaceCapabilities.currentExtent.width != UINT32_MAX) {
            // currentExtentが特定の値でない場合は、サーフェイスの能力のcurrentExtentをそのまま選択する
            return surfaceCapabilities.currentExtent;
        }
        // currentExtentが特定の値の場合は、サーフェイスの能力のminImageExtentとmaxImageExtentの範囲内で,
        // ウィンドウのフレームバッファのサイズに最も近いエクステントを選択する
        const auto minImageExtent = surfaceCapabilities.minImageExtent;
        const auto maxImageExtent = surfaceCapabilities.maxImageExtent;
        VkExtent2D actualExtent = {
            .width = clampToRange(_frameBufferSize.getWidthValue(), minImageExtent.width, maxImageExtent.width),
            .height = clampToRange(_frameBufferSize.getHeightValue(), minImageExtent.height, maxImageExtent.height),
        };
        return actualExtent;
    }
}
