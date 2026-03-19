//
// Created by 沖田大河 on 2026/03/15.
//

#ifndef TUTORIAL_VULKANPHYSICALDEVICEFEATUREREQUIREMENTS_H
#define TUTORIAL_VULKANPHYSICALDEVICEFEATUREREQUIREMENTS_H
#include "VulkanPhysicalDevice.h"

namespace Tutorial::Graphics {

    /**
     *  物理デバイス機能に対する要求を表すクラス
     */
    class VulkanPhysicalDeviceFeatureRequirements {

        VkPhysicalDeviceExtendedDynamicStateFeaturesEXT _extendedDynamicStateFeatures{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
            .pNext = nullptr,
            .extendedDynamicState = VK_TRUE,
        };

        VkPhysicalDeviceVulkan13Features _vulkan13Features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .pNext = &_extendedDynamicStateFeatures,
            .dynamicRendering = VK_TRUE,
        };

        VkPhysicalDeviceFeatures2 _requiredFeatures{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &_vulkan13Features,
        };

    public:
        VulkanPhysicalDeviceFeatureRequirements() = default;

        /**
         * 物理デバイスが必要な機能をサポートしているか
         * @param physicalDevice 確認対象の物理デバイス
         * @return 必要な機能をサポートしているならtrue、そうでないならfalse
         */
        bool isSatisfiedBy(const VulkanPhysicalDevice &physicalDevice) const;


        /**
         * この要求をVkPhysicalDeviceFeatures2構造体の形で表して返す
         * @return 必要としている機能を有効化したVkPhysicalDeviceFeatures2構造体
         */
        const VkPhysicalDeviceFeatures2& asDeviceFeature() const;
    };

}

#endif //TUTORIAL_VULKANPHYSICALDEVICEFEATUREREQUIREMENTS_H