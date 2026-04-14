//
// Created by 沖田大河 on 2026/04/11.
//

#include "VulkanSwapChainCreateStrategy.h"

#include <string>

namespace Tutorial::Graphics {
    VkSurfaceCapabilitiesKHR VulkanSwapChainCreateStrategy::getSurfaceCapabilities(
        const VulkanPhysicalDevice &physicalDevice, const VulkanSurface &vulkanSurface) const {
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        if (physicalDevice.getSurfaceCapabilitiesKHR(vulkanSurface.getSurface(), &surfaceCapabilities) != VK_SUCCESS) {
            throw std::runtime_error("Failed to get surface capabilities");
        }
        return surfaceCapabilities;
    }

    Span<VkSurfaceFormatKHR> VulkanSwapChainCreateStrategy::getSurfaceFormats(
        const VulkanPhysicalDevice &physicalDevice, const VulkanSurface &vulkanSurface) const {
        uint32_t surfaceFormatCount = 0;
        if (const auto result = physicalDevice.getSurfaceFormatsKHR(vulkanSurface.getSurface(), &surfaceFormatCount, nullptr); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to get surface formats count: " + std::to_string(result));
        }
        auto surfaceFormats = Span<VkSurfaceFormatKHR>::stackAlloc(surfaceFormatCount);
        if (const auto result = physicalDevice.getSurfaceFormatsKHR(vulkanSurface.getSurface(), &surfaceFormatCount, surfaceFormats.getHeadPtr()); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to get surface formats: " + std::to_string(result));
        }
        surfaceFormats.markFilled(surfaceFormatCount);
        return surfaceFormats;
    }

    Span<VkPresentModeKHR> VulkanSwapChainCreateStrategy::getPresentModes(const VulkanPhysicalDevice &physicalDevice,
        const VulkanSurface &vulkanSurface) const {
        uint32_t presentModeCount = 0;
        if (physicalDevice.getPhysicalDeviceSurfacePresentModeKHR(vulkanSurface.getSurface(), &presentModeCount, nullptr) != VK_SUCCESS) {
            throw std::runtime_error("Failed to get present modes count");
        }
        auto presentModes = Span<VkPresentModeKHR>::stackAlloc(presentModeCount);
        if (physicalDevice.getPhysicalDeviceSurfacePresentModeKHR(vulkanSurface.getSurface(), &presentModeCount, presentModes.getHeadPtr()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to get present modes");
        }
        presentModes.markFilled(presentModeCount);
        return presentModes;
    }

    VulkanSwapChainCreateStrategy::VulkanSwapChainCreateStrategy(FrameBufferSize frameBufferSize):
        _surfaceExtentRequirements(frameBufferSize) {
    }

     VulkanSwapChain VulkanSwapChainCreateStrategy::createSwapChain(
         const VulkanPhysicalDevice &physicalDevice,
         const VulkanSurface &vulkanSurface,
         const VulkanLogicalDevice &logicalDevice) const {
        const auto surfaceCapabilities = getSurfaceCapabilities(physicalDevice, vulkanSurface);
        const auto surfaceFormats = getSurfaceFormats(physicalDevice, vulkanSurface);
        const auto presentModes = getPresentModes(physicalDevice, vulkanSurface);

        const auto surfaceFormat = _surfaceFormatRequirements.chooseSatisfiedOne(surfaceFormats);
        const auto surfaceExtent = _surfaceExtentRequirements.chooseSatisfiedOne(surfaceCapabilities);
        const auto presentMode = _presentModeRequirements.chooseSatisfiedOne(presentModes);
        const auto imageCount = _imageCountSelectStrategy.chooseImageCount(surfaceCapabilities);
        const auto swapChainCreateInfo = VkSwapchainCreateInfoKHR {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .surface = vulkanSurface.getSurface(),
            .minImageCount = imageCount,
            .imageFormat = surfaceFormat.format,
            .imageColorSpace = surfaceFormat.colorSpace,
            .imageExtent = surfaceExtent,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr,
            .preTransform = surfaceCapabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = presentMode,
            .clipped = VK_TRUE,
            .oldSwapchain = VK_NULL_HANDLE,
        };

        return VulkanSwapChain(swapChainCreateInfo, logicalDevice);
     }
}
