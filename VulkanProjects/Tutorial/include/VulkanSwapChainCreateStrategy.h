//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_VULKAN_SWAP_CHAIN_CREATE_STRATEGY_H
#define TUTORIAL_VULKAN_SWAP_CHAIN_CREATE_STRATEGY_H

#include "FrameBufferSize.h"
#include "SwapChainPresentModeRequirements.h"
#include "SwapChainVulkanSurfaceExtentRequirements.h"
#include "SwapChainVulkanSurfaceFormatRequirements.h"
#include "SwapChainImageCountSelectStrategy.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSwapChain.h"

namespace Tutorial::Graphics {

    /**
     * SwapChainの作成戦略を表すクラス
     */
    class VulkanSwapChainCreateStrategy {
        const SwapChainPresentModeRequirements _presentModeRequirements;
        const SwapChainVulkanSurfaceFormatRequirements _surfaceFormatRequirements;
        const SwapChainVulkanSurfaceExtentRequirements _surfaceExtentRequirements;
        const SwapChainImageCountSelectStrategy _imageCountSelectStrategy;

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