//
// Created by 沖田大河 on 2026/04/11.
//

#include "VulkanSwapChain.h"

#include <string>

#include "VulkanLogicalDevice.h"

namespace Tutorial::Graphics {
    VkSwapchainKHR VulkanSwapChain::acquisitionSwapChainResource(VkDevice vkDevice, const VkSwapchainCreateInfoKHR &createInfo) const {
        VkSwapchainKHR swapChain;
        if (const auto result = vkCreateSwapchainKHR(vkDevice, &createInfo, nullptr, &swapChain); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create swap chain: " + std::to_string(result));
        }
        return swapChain;
    }

    void VulkanSwapChain::cacheProperties(const VkSwapchainCreateInfoKHR &createInfo) {
        _surfaceFormat = VkSurfaceFormatKHR{
            .format = createInfo.imageFormat,
            .colorSpace = createInfo.imageColorSpace,
        };
        _extent = createInfo.imageExtent;
    }

    VulkanSwapChain::VulkanSwapChain(VkDevice vkDevice, const VkSwapchainCreateInfoKHR &createInfo):
        _handle(acquisitionSwapChainResource(vkDevice, createInfo)),
        _vkDevice(vkDevice) {
        cacheProperties(createInfo);
    }

    VkSwapchainKHR VulkanSwapChain::getHandle() const {
        return _handle;
    }

    Span<VkImage> VulkanSwapChain::getImages() const {
        uint32_t imageCount = 0;
        if (vkGetSwapchainImagesKHR(_vkDevice, _handle, &imageCount, nullptr) != VK_SUCCESS) {
            throw std::runtime_error("Failed to get swap chain images count");
        }
        auto images = Span<VkImage>::stackAlloc(imageCount);
        if (vkGetSwapchainImagesKHR(_vkDevice, _handle, &imageCount, images.getHeadPtr()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to get swap chain images");
        }
        images.markFilled(imageCount);
        return images;
    }

    VulkanSwapChain::VulkanSwapChain(VulkanSwapChain &&other) noexcept {
        _handle = other._handle;
        _vkDevice = other._vkDevice;
        _surfaceFormat = other._surfaceFormat;
        _extent = other._extent;

        other._handle = VK_NULL_HANDLE;
    }

    VulkanSwapChain & VulkanSwapChain::operator=(VulkanSwapChain &&other) noexcept {
        if (this != &other) {
            if (_handle != VK_NULL_HANDLE) {
                vkDestroySwapchainKHR(_vkDevice, _handle, nullptr);
            }
            _handle = other._handle;
            _vkDevice = other._vkDevice;
            _surfaceFormat = other._surfaceFormat;
            _extent = other._extent;

            other._handle = VK_NULL_HANDLE;
        }
        return *this;
    }

    VulkanSwapChain::~VulkanSwapChain() {
        if (_handle != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(_vkDevice, _handle, nullptr);
        }
    }
} // Graphics
