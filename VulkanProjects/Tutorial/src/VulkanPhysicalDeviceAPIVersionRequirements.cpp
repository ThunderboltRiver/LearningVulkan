//
// Created by 沖田大河 on 2026/03/15.
//

#include "../include/VulkanPhysicalDeviceAPIVersionRequirements.h"

namespace Tutorial::Graphics {
    bool VulkanPhysicalDeviceAPIVersionRequirements::isSatisfiedBy(const VulkanPhysicalDevice &physicalDevice) const {
        VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeatures{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
            .pNext = nullptr,
        };
        VkPhysicalDeviceVulkan13Features vulkan13Features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .pNext = &extendedDynamicStateFeatures,
        };

        VkPhysicalDeviceProperties2 properties2 {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
            .pNext = &vulkan13Features,
        };
        physicalDevice.getProperties2(properties2);
        return properties2.properties.apiVersion >= VK_API_VERSION_1_3;
    }
}