//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_VULKAN_SWAP_CHAIN_CREATE_STRATEGY_H
#define TUTORIAL_VULKAN_SWAP_CHAIN_CREATE_STRATEGY_H

#include "VulkanLogicalDevice.h"
#include "Graphics/FrameBufferSize.h"
#include "Graphics/SwapChainPresentModePreference.h"
#include "Graphics/SwapChainVulkanSurfaceExtentPreference.h"
#include "Graphics/SwapChainVulkanSurfaceFormatPreference.h"
#include "Graphics/SwapChainImageCountPreference.h"
#include "Graphics/VulkanPhysicalDevice.h"
#include "Graphics/VulkanSurface.h"
#include "Graphics/VulkanSwapChain.h"

namespace Tutorial::Graphics {

    /**
     * SwapChainの作成戦略を表すクラス
     */
    class VulkanSwapChainCreateStrategy {
        const SwapChainPresentModePreference _presentModePreference;
        const SwapChainVulkanSurfaceFormatPreference _surfaceFormatPreference;
        const SwapChainVulkanSurfaceExtentPreference _surfaceExtentPreference;
        const SwapChainImageCountPreference _imageCountPreference;

        [[nodiscard]] VkSurfaceCapabilitiesKHR getSurfaceCapabilities(const VulkanPhysicalDevice &physicalDevice,
                                                                      const VulkanSurface &vulkanSurface) const;
        [[nodiscard]] Span<VkSurfaceFormatKHR> getSurfaceFormats(const VulkanPhysicalDevice &physicalDevice,
                                                                 const VulkanSurface &vulkanSurface) const;
        [[nodiscard]] Span<VkPresentModeKHR> getPresentModes(const VulkanPhysicalDevice &physicalDevice, const VulkanSurface &vulkanSurface) const;

    public:
        explicit VulkanSwapChainCreateStrategy(FrameBufferSize frameBufferSize);

        [[nodiscard]] VulkanSwapChain createSwapChain(const VulkanPhysicalDevice &physicalDevice, const VulkanSurface &vulkanSurface, const VulkanLogicalDevice &logicalDevice) const;
    };
}
#endif //TUTORIAL_VULKAN_SWAP_CHAIN_CREATE_STRATEGY_H