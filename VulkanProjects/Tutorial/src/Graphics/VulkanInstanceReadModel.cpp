//
// Created by 沖田大河 on 2026/04/18.
//

#include "Graphics/VulkanInstanceReadModel.h"

#include "Graphics/VulkanPhysicalDevice.h"

namespace Tutorial::Graphics {
    VulkanInstanceReadModel::VulkanInstanceReadModel(Borrowed<VkInstance> vkInstance):
        _vkInstance(vkInstance) {
    }

    Span<VulkanPhysicalDevice> VulkanInstanceReadModel::enumeratePhysicalDevices() const {
        uint32_t physicalDeviceCount = 0;
        if (vkEnumeratePhysicalDevices(_vkInstance.getRawHandle(), &physicalDeviceCount, nullptr) != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate physical devices count");
        }
        auto physicalDevices = Span<VkPhysicalDevice>::stackAlloc(physicalDeviceCount);
        if (vkEnumeratePhysicalDevices(_vkInstance.getRawHandle(), &physicalDeviceCount, physicalDevices.getHeadPtr()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate physical devices");
        }
        physicalDevices.markFilled(physicalDeviceCount);

        auto results = Span<VulkanPhysicalDevice>::stackAlloc(physicalDeviceCount);
        for (auto physicalDevice: physicalDevices) {
            results.Add(VulkanPhysicalDevice(Borrowed(physicalDevice)));
        }
        return results;
    }
}
