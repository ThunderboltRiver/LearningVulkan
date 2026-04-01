//
// Created by 沖田大河 on 2026/03/14.
//

#include "VulkanPhysicalDevice.h"

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
} // Graphics