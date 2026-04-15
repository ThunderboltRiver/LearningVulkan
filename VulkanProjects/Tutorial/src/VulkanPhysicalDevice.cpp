//
// Created by 沖田大河 on 2026/03/14.
//

#include "VulkanPhysicalDevice.h"

#include <stdexcept>
#include <string>

namespace Tutorial::Graphics {
    VkPhysicalDevice VulkanPhysicalDevice::getHandle() const {
        return _physicalDevice;
    }

    void VulkanPhysicalDevice::getProperties2(VkPhysicalDeviceProperties2& properties) const {
        vkGetPhysicalDeviceProperties2(_physicalDevice, &properties);
    }

    void VulkanPhysicalDevice::getQueueFamilyProperties2(uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) const {
        vkGetPhysicalDeviceQueueFamilyProperties2(_physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    }

    VkResult VulkanPhysicalDevice::enumerateExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) const {
        return vkEnumerateDeviceExtensionProperties(_physicalDevice, pLayerName, pPropertyCount, pProperties);
    }

    void VulkanPhysicalDevice::getFeatures2(VkPhysicalDeviceFeatures2& features) const {
        vkGetPhysicalDeviceFeatures2(_physicalDevice, &features);
    }

    VkResult VulkanPhysicalDevice::createDevice(const VkDeviceCreateInfo &deviceCreateInfo, const VkAllocationCallbacks *pAllocator, VkDevice *pLogicalDevice) const {
        return vkCreateDevice(_physicalDevice, &deviceCreateInfo, pAllocator, pLogicalDevice);
    }

    VkResult VulkanPhysicalDevice::getSurfaceSupportKHR(uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32 *pSupported) const {
        return vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, queueFamilyIndex, surface, pSupported);
    }

    VkResult VulkanPhysicalDevice::getSurfaceCapabilitiesKHR(VkSurfaceKHR surface,
        VkSurfaceCapabilitiesKHR *pSurfaceCapabilities) const {
        return vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice, surface, pSurfaceCapabilities);
    }

    VkResult VulkanPhysicalDevice::getSurfaceCapabilities2KHR(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) const {
        return vkGetPhysicalDeviceSurfaceCapabilities2KHR(_physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
    }

    VkResult VulkanPhysicalDevice::getSurfaceFormatsKHR(VkSurfaceKHR surface,  uint32_t *pSurfaceFormatCount, VkSurfaceFormatKHR *pSurfaceFormat) const {
        return vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormat);
    }

    VkResult VulkanPhysicalDevice::getPhysicalDeviceSurfaceFormats2KHR(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) const {
        return vkGetPhysicalDeviceSurfaceFormats2KHR(_physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
    }

    VkResult VulkanPhysicalDevice::getPhysicalDeviceSurfacePresentModeKHR(VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) const {
        return vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice, surface, pPresentModeCount, pPresentModes);
    }

#ifdef VK_USE_PLATFORM_WIN32_KHR
    VkResult VulkanPhysicalDevice::getPhysicalDeviceSurfacePresentModes2Ext(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) const {
        return vkGetPhysicalDeviceSurfacePresentModes2EXT(_physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
    }
#endif



    Span<VkExtensionProperties> VulkanPhysicalDevice::enumerateExtensionProperties(char const* pLayerName) const {
        uint32_t extensionCount = 0;
        if (vkEnumerateDeviceExtensionProperties(_physicalDevice, pLayerName, &extensionCount, nullptr) != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate device extension properties");
        }
        auto extensions = Span<VkExtensionProperties>::stackAlloc(extensionCount);
        if (vkEnumerateDeviceExtensionProperties(_physicalDevice, pLayerName, &extensionCount, extensions.getHeadPtr()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate device extension properties");
        }
        extensions.markFilled();
        return extensions;
    }

    bool VulkanPhysicalDevice::isPresentationSupported(uint32_t queueFamilyIndex, VkSurfaceKHR surface) const {
        VkBool32 isSupported;
        if (const auto result = getSurfaceSupportKHR(queueFamilyIndex, surface, &isSupported); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to get surface support for queue family index " + std::to_string(queueFamilyIndex) + ": " + std::to_string(result));
        }
        return isSupported == VK_TRUE;
    }
} // Graphics