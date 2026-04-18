//
// Created by 沖田大河 on 2026/03/14.
//

#include "Graphics/VulkanPhysicalDeviceSelectionStrategy.h"

#include "Debug/Logger.h"

namespace Tutorial::Graphics {

    VulkanPhysicalDevice VulkanPhysicalDeviceSelectionStrategy::selectPhysicalDevice(VulkanInstanceReadModel vulkanInstanceReadModel) const {
        const auto vulkanPhysicalDevices = vulkanInstanceReadModel.enumeratePhysicalDevices();
        for (const auto& vulkanPhysicalDevice: vulkanPhysicalDevices) {
            uint32_t queueFamilyIndex;
            if (_apiVersionRequirements.isSatisfiedBy(vulkanPhysicalDevice)
                && _queueFamilyRequirements.findSatisfiedQueueFamilyIndex(vulkanPhysicalDevice, &queueFamilyIndex)
                && _deviceExtensionRequirements.isSatisfiedBy(vulkanPhysicalDevice)
                && _deviceFeatureRequirements.isSatisfiedBy(vulkanPhysicalDevice)) {
                Debug::Logger::log("Selected physical device that supports required Vulkan API version, queue family properties, device extensions and device features");
                return vulkanPhysicalDevice;
            }
        }
        throw std::runtime_error("Failed to find a suitable GPU that satisfies all requirements");
    }
}
