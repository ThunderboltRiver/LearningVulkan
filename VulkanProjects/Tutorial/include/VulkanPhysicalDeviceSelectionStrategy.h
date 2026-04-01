//
// Created by 沖田大河 on 2026/03/14.
//

#ifndef TUTORIAL_VULKAN_PHYSICAL_DEVICE_SELECTION_STRATEGY_H
#define TUTORIAL_VULKAN_PHYSICAL_DEVICE_SELECTION_STRATEGY_H

#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanPhysicalDeviceAPIVersionRequirements.h"
#include "VulkanPhysicalDeviceQueueFamilyRequirements.h"
#include "VulkanPhysicalDeviceExtensionsRequirements.h"
#include "VulkanPhysicalDeviceFeatureRequirements.h"

namespace Tutorial::Graphics {

    /**
     * 物理デバイス選択戦略を表すクラス
     */
    class VulkanPhysicalDeviceSelectionStrategy {

        const VulkanInstance &_vulkanInstance;
        const VulkanPhysicalDeviceAPIVersionRequirements _apiVersionRequirements;
        const VulkanPhysicalDeviceQueueFamilyRequirements _queueFamilyRequirements;
        const VulkanPhysicalDeviceFeatureRequirements _deviceFeatureRequirements;
        const VulkanPhysicalDeviceExtensionsRequirements _deviceExtensionRequirements;

    public:
        explicit VulkanPhysicalDeviceSelectionStrategy(
            const VulkanInstance &vulkanInstance
            ):
            _vulkanInstance(vulkanInstance){
        }

        /**
         * VulkanInstanceから物理デバイスを選択する
         * @return 選択された物理デバイスのハンドル
         */
        [[nodiscard]] VulkanPhysicalDevice selectPhysicalDevice() const;
    };
}
#endif //TUTORIAL_VULKAN_PHYSICAL_DEVICE_SELECTION_STRATEGY_H