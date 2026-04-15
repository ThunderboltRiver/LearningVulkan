//
// Created by 沖田大河 on 2026/03/20.
//

#include "Graphics/VulkanLogicalDeviceCreationStrategy.h"

namespace Tutorial::Graphics {
    VulkanLogicalDeviceCreationStrategy::VulkanLogicalDeviceCreationStrategy(
        const VulkanPhysicalDeviceQueueFamilyRequirements &queueFamilyRequirements,
        const VulkanPhysicalDeviceFeatureRequirements &deviceFeatureRequirements,
        const VulkanPhysicalDeviceExtensionsRequirements &deviceExtensionRequirements):
        _queueFamilyRequirements(queueFamilyRequirements),
        _deviceFeatureRequirements(deviceFeatureRequirements),
        _deviceExtensionRequirements(deviceExtensionRequirements) {

    }

    VulkanLogicalDevice VulkanLogicalDeviceCreationStrategy::createLogicalDevice(const VulkanPhysicalDevice &physicalDevice) const {
        uint32_t graphicsQueueFamilyIndex;
        if (!_queueFamilyRequirements.findSatisfiedQueueFamilyIndex(physicalDevice, &graphicsQueueFamilyIndex)) {
            throw std::runtime_error("Failed to find a queue family that supports graphics commands");
        }
        const auto deviceExtensions = _deviceExtensionRequirements.asVkDeviceExtensionNames(physicalDevice);
        float queuePriority = 0.5f;
        VkDeviceQueueCreateInfo queueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = graphicsQueueFamilyIndex,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority,
        };
        VkDeviceCreateInfo deviceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = &_deviceFeatureRequirements.asDeviceFeature(),
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &queueCreateInfo,
            .enabledExtensionCount = deviceExtensions.getElementCount(),
            .ppEnabledExtensionNames = deviceExtensions.getHeadPtr(),
        };

        VulkanLogicalDevice logicalDevice(physicalDevice, deviceCreateInfo);
        return logicalDevice;
    }
}
