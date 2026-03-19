//
// Created by 沖田大河 on 2026/03/14.
//

#ifndef TUTORIAL_VULKAN_PHYSICAL_DEVICE_H
#define TUTORIAL_VULKAN_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>

namespace Tutorial::Graphics {

    /**
     * Vulkanの物理デバイスを表すクラス
     */
    class VulkanPhysicalDevice {

        // 物理デバイスのハンドル
        const VkPhysicalDevice& _physicalDevice;

    public:
        explicit VulkanPhysicalDevice(const VkPhysicalDevice& physicalDevice) : _physicalDevice(physicalDevice) {}

        void getProperties2(VkPhysicalDeviceProperties2& properties) const;

        void getQueueFamilyProperties2(uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) const;

        void enumerateExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) const;

        void getFeatures2(VkPhysicalDeviceFeatures2& features) const;
    };
} // Graphics

#endif //TUTORIAL_VULKAN_PHYSICAL_DEVICE_H