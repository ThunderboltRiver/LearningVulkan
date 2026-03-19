//
// Created by 沖田大河 on 2026/03/15.
//

#include "VulkanPhysicalDeviceFeatureRequirements.h"

namespace Tutorial::Graphics {
    bool VulkanPhysicalDeviceFeatureRequirements::isSatisfiedBy(const VulkanPhysicalDevice &physicalDevice) const {
        // 物理デバイスがサポートする機能の構造体を取得する
        VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeatures{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
            .pNext = nullptr,
        };

        VkPhysicalDeviceVulkan13Features vulkan13Features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .pNext = &extendedDynamicStateFeatures,
        };

        VkPhysicalDeviceFeatures2 supportedFeatures{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &vulkan13Features,
        };
        physicalDevice.getFeatures2(supportedFeatures);

        // 物理デバイスがサポートする機能の中に、必要な機能がすべてサポートされているかを確認する
        return vulkan13Features.dynamicRendering && extendedDynamicStateFeatures.extendedDynamicState;
    }

    const VkPhysicalDeviceFeatures2& VulkanPhysicalDeviceFeatureRequirements::asDeviceFeature() const {
        VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeatures{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
            .pNext = nullptr,
            .extendedDynamicState = VK_TRUE,
        };

        VkPhysicalDeviceVulkan13Features vulkan13Features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .pNext = &extendedDynamicStateFeatures,
            .dynamicRendering = VK_TRUE,
        };

        const VkPhysicalDeviceFeatures2& supportedFeatures{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &vulkan13Features,
        };
        return supportedFeatures;
    }
} // Graphics
