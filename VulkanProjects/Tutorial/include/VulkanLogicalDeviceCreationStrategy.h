//
// Created by 沖田大河 on 2026/03/20.
//

#ifndef TUTORIAL_VULKAN_LOGICAL_DEVICE_CREATION_STRATEGY_H
#define TUTORIAL_VULKAN_LOGICAL_DEVICE_CREATION_STRATEGY_H

#include <vulkan/vulkan.h>

#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanPhysicalDeviceExtensionsRequirements.h"
#include "VulkanPhysicalDeviceFeatureRequirements.h"
#include "VulkanPhysicalDeviceQueueFamilyRequirements.h"

namespace Tutorial::Graphics {

    /**
     * 論理デバイスの作成戦略を表すクラス
     */
    class VulkanLogicalDeviceCreationStrategy {

        const VulkanPhysicalDeviceQueueFamilyRequirements _queueFamilyRequirements;
        const VulkanPhysicalDeviceFeatureRequirements _deviceFeatureRequirements;
        const VulkanPhysicalDeviceExtensionsRequirements _deviceExtensionRequirements;

    public:

        explicit VulkanLogicalDeviceCreationStrategy(
            const VulkanPhysicalDeviceQueueFamilyRequirements& queueFamilyRequirements,
            const VulkanPhysicalDeviceFeatureRequirements& deviceFeatureRequirements,
            const VulkanPhysicalDeviceExtensionsRequirements& deviceExtensionRequirements
        ) :
        _queueFamilyRequirements(queueFamilyRequirements),
        _deviceFeatureRequirements(deviceFeatureRequirements),
        _deviceExtensionRequirements(deviceExtensionRequirements) {

        }

        VulkanLogicalDeviceCreationStrategy() = default;

        /**
         * 物理デバイスから論理デバイスを作成する
         * @param physicalDevice 物理デバイス
         * @return 物理デバイスから論理デバイスを作成して返す
         */
        [[nodiscard]] VulkanLogicalDevice createLogicalDevice(const VulkanPhysicalDevice& physicalDevice) const;
    };
}

#endif //TUTORIAL_VULKAN_LOGICAL_DEVICE_CREATION_STRATEGY_H