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

    VulkanSwapChain::VulkanSwapChain(VkDevice vkDevice, const VkSwapchainCreateInfoKHR &createInfo):
        _handle(acquisitionSwapChainResource(vkDevice, createInfo)),
        _vkDevice(vkDevice) {
    }

    VkSwapchainKHR VulkanSwapChain::getHandle() const {
        return _handle;
    }

    VulkanSwapChain::VulkanSwapChain(VulkanSwapChain &&other) noexcept {
        _handle = other._handle;
        _vkDevice = other._vkDevice;
        other._handle = VK_NULL_HANDLE;
    }

    VulkanSwapChain & VulkanSwapChain::operator=(VulkanSwapChain &&other) noexcept {
        if (this != &other) {
            if (_handle != VK_NULL_HANDLE) {
                vkDestroySwapchainKHR(_vkDevice, _handle, nullptr);
            }
            _handle = other._handle;
            _vkDevice = other._vkDevice;
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
