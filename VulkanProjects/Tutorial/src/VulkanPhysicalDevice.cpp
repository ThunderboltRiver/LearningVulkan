//
// Created by 沖田大河 on 2026/03/14.
//

#include "VulkanPhysicalDevice.h"

#include <stdexcept>
#include <string>

namespace Tutorial::Graphics {
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

    bool VulkanPhysicalDevice::isPresentationSupported(uint32_t queueFamilyIndex, VkSurfaceKHR surface) const {
        VkBool32 isSupported;
        if (getSurfaceSupportKHR(queueFamilyIndex, surface, &isSupported) != VK_SUCCESS) {
            throw std::runtime_error("Failed to get surface support for queue family index " + std::to_string(queueFamilyIndex));
        }
        return isSupported == VK_TRUE;
    }
} // Graphics