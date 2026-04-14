//
// Created by 沖田大河 on 2026/04/11.
//

#include "SwapChainVulkanSurfaceFormatRequirements.h"

namespace Tutorial::Graphics {

    SwapChainVulkanSurfaceFormatRequirements::SwapChainVulkanSurfaceFormatRequirements() = default;

    bool SwapChainVulkanSurfaceFormatRequirements::isSatisfiedBy(VkSurfaceFormatKHR surfaceFormat) const {
        // フォーマットがVK_FORMAT_B8G8R8A8_SRGBで、色空間がVK_COLOR_SPACE_SRGB_NONLINEAR_KHRであることを要求する
        return surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB
            && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    }

    VkSurfaceFormatKHR SwapChainVulkanSurfaceFormatRequirements::chooseSatisfiedOne(const Span<VkSurfaceFormatKHR> &surfaceFormats) const {
        for (const auto& surfaceFormat : surfaceFormats) {
            if (isSatisfiedBy(surfaceFormat)) {
                // 要件を満たすサーフェスのフォーマットが存在する場合はそれを選択する
                return surfaceFormat;
            }
        }
        throw std::runtime_error("Failed to find a surface format that satisfies the requirements");
    }

} // Graphics
