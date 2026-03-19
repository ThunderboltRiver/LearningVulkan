//
// Created by 沖田大河 on 2026/03/14.
//

#include "VulkanPhysicalDeviceSelectionStrategy.h"

#include <cstring>

#include "Logger.h"

namespace Tutorial::Graphics {

    VulkanPhysicalDevice VulkanPhysicalDeviceSelectionStrategy::selectPhysicalDevice() const {
        const auto physicalDeviceCount = _vulkanInstance.getPhysicalDevicesCount();
        if (physicalDeviceCount == 0) {
            throw std::runtime_error("Failed to find GPUs with Vulkan support");
        }
        const auto physicalDevices = Span<VkPhysicalDevice>::stackAlloc(physicalDeviceCount);
        _vulkanInstance.enumeratePhysicalDevices(physicalDevices);

        for (const auto& physicalDevice : physicalDevices) {
            VulkanPhysicalDevice vulkanPhysicalDevice(physicalDevice);
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
