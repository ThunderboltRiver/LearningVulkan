//
// Created by 沖田大河 on 2026/03/14.
//

#ifndef TUTORIAL_VULKAN_PHYSICAL_DEVICE_SELECTION_STRATEGY_H
#define TUTORIAL_VULKAN_PHYSICAL_DEVICE_SELECTION_STRATEGY_H

#include "Graphics/VulkanInstance.h"
#include "Graphics/VulkanPhysicalDevice.h"
#include "Graphics/VulkanPhysicalDeviceAPIVersionRequirements.h"
#include "Graphics/VulkanPhysicalDeviceQueueFamilyRequirements.h"
#include "Graphics/VulkanPhysicalDeviceExtensionsRequirements.h"
#include "Graphics/VulkanPhysicalDeviceFeatureRequirements.h"

namespace Tutorial::Graphics {

    /**
     * 物理デバイス選択戦略を表すクラス
     */
    class VulkanPhysicalDeviceSelectionStrategy {

        const VulkanInstance &_vulkanInstance;
        const VulkanPhysicalDeviceAPIVersionRequirements& _apiVersionRequirements;
        const VulkanPhysicalDeviceQueueFamilyRequirements& _queueFamilyRequirements;
        const VulkanPhysicalDeviceFeatureRequirements& _deviceFeatureRequirements;
        const VulkanPhysicalDeviceExtensionsRequirements& _deviceExtensionRequirements;

    public:
        explicit VulkanPhysicalDeviceSelectionStrategy(
            const VulkanInstance &vulkanInstance,
            const VulkanPhysicalDeviceAPIVersionRequirements& apiVersionRequirements,
            const VulkanPhysicalDeviceQueueFamilyRequirements& queueFamilyRequirements,
            const VulkanPhysicalDeviceFeatureRequirements& deviceFeatureRequirements,
            const VulkanPhysicalDeviceExtensionsRequirements& deviceExtensionRequirements):
            _vulkanInstance(vulkanInstance),
            _apiVersionRequirements(apiVersionRequirements),
            _queueFamilyRequirements(queueFamilyRequirements),
            _deviceFeatureRequirements(deviceFeatureRequirements),
            _deviceExtensionRequirements(deviceExtensionRequirements) {
        }

        /**
         * VulkanInstanceから物理デバイスを選択する
         * @return 選択された物理デバイスのハンドル
         */
        [[nodiscard]] VulkanPhysicalDevice selectPhysicalDevice() const;
    };
}
#endif //TUTORIAL_VULKAN_PHYSICAL_DEVICE_SELECTION_STRATEGY_H