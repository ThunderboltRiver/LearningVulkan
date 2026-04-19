//
// Created by 沖田大河 on 2026/04/11.
//

#include "Graphics/VulkanSwapChain.h"

#include <string>

#include "Graphics/VulkanLogicalDevice.h"
#include "ResourceManagement.h"

namespace Tutorial::Graphics {
    OwnerShip<VkSwapchainKHR> VulkanSwapChain::resourceAcquisition(Borrowed<VkDevice> vkDevice, const VkSwapchainCreateInfoKHR &createInfo) const {
        VkSwapchainKHR swapChain;
        if (const auto result = vkCreateSwapchainKHR(vkDevice.getRawHandle(), &createInfo, nullptr, &swapChain); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create swap chain: " + std::to_string(result));
        }
        return OwnerShip(swapChain);
    }

    void VulkanSwapChain::cacheProperties(const VkSwapchainCreateInfoKHR &createInfo) {
        _surfaceFormat = VkSurfaceFormatKHR{
            .format = createInfo.imageFormat,
            .colorSpace = createInfo.imageColorSpace,
        };
        _extent = createInfo.imageExtent;
    }

    VulkanSwapChain::VulkanSwapChain(Borrowed<VkDevice> vkDevice, const VkSwapchainCreateInfoKHR &createInfo):
        _pSwapChain(resourceAcquisition(vkDevice, createInfo)),
        _vkDevice(vkDevice) {
        cacheProperties(createInfo);
    }

    Borrowed<VkSwapchainKHR> VulkanSwapChain::getHandle() const {
        return _pSwapChain.borrow();
    }

    Span<VkImage> VulkanSwapChain::getImages() const {
        uint32_t imageCount = 0;
        if (vkGetSwapchainImagesKHR(_vkDevice.getRawHandle(), _pSwapChain.getRawHandle(), &imageCount, nullptr) != VK_SUCCESS) {
            throw std::runtime_error("Failed to get swap chain images count");
        }
        auto images = Span<VkImage>::stackAlloc(imageCount);
        if (vkGetSwapchainImagesKHR(_vkDevice.getRawHandle(), _pSwapChain.getRawHandle(), &imageCount, images.getHeadPtr()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to get swap chain images");
        }
        images.markFilled(imageCount);
        return images;
    }

    VulkanSwapChain::VulkanSwapChain(VulkanSwapChain &&other) noexcept:
        _pSwapChain(other._pSwapChain.move()),
        _vkDevice(other._vkDevice),
        _surfaceFormat(other._surfaceFormat),
        _extent(other._extent) {
        other._pSwapChain = OwnerShip<VkSwapchainKHR>::MOVED();
    }

    VulkanSwapChain & VulkanSwapChain::operator=(VulkanSwapChain &&other) noexcept {
        if (this != &other) {
            if (_pSwapChain != OwnerShip<VkSwapchainKHR>::MOVED()) {
                vkDestroySwapchainKHR(_vkDevice.getRawHandle(), _pSwapChain.getRawHandle(), nullptr);
            }
            _pSwapChain = other._pSwapChain.move();
            _vkDevice = other._vkDevice;
            _surfaceFormat = other._surfaceFormat;
            _extent = other._extent;

            other._pSwapChain = OwnerShip<VkSwapchainKHR>::MOVED();
        }
        return *this;
    }

    VulkanSwapChain::~VulkanSwapChain() {
        if (_pSwapChain.isNotMoved()) {
            vkDestroySwapchainKHR(_vkDevice.getRawHandle(), _pSwapChain.getRawHandle(), nullptr);
        }
    }
} // Graphics
