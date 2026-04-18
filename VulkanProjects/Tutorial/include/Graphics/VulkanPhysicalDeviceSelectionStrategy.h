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
        const VulkanPhysicalDeviceAPIVersionRequirements& _apiVersionRequirements;
        const VulkanPhysicalDeviceQueueFamilyRequirements& _queueFamilyRequirements;
        const VulkanPhysicalDeviceFeatureRequirements& _deviceFeatureRequirements;
        const VulkanPhysicalDeviceExtensionsRequirements& _deviceExtensionRequirements;

    public:
        explicit VulkanPhysicalDeviceSelectionStrategy(
            const VulkanPhysicalDeviceAPIVersionRequirements& apiVersionRequirements,
            const VulkanPhysicalDeviceQueueFamilyRequirements& queueFamilyRequirements,
            const VulkanPhysicalDeviceFeatureRequirements& deviceFeatureRequirements,
            const VulkanPhysicalDeviceExtensionsRequirements& deviceExtensionRequirements):
            _apiVersionRequirements(apiVersionRequirements),
            _queueFamilyRequirements(queueFamilyRequirements),
            _deviceFeatureRequirements(deviceFeatureRequirements),
            _deviceExtensionRequirements(deviceExtensionRequirements) {
        }

        /**
         * VulkanInstanceから物理デバイスを選択する
         * @param vulkanInstanceReadModel VulkanInstanceのReadModel
         * @return VulkanInstanceから選択された物理デバイス
         */
        [[nodiscard]] VulkanPhysicalDevice selectPhysicalDevice(VulkanInstanceReadModel vulkanInstanceReadModel) const;
    };
}
#endif //TUTORIAL_VULKAN_PHYSICAL_DEVICE_SELECTION_STRATEGY_H