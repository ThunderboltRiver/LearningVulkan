//
// Created by 沖田大河 on 2026/03/19.
//

#include "../include/VulkanLogicalDevice.h"

#include <stdexcept>

namespace Tutorial::Graphics {
    VkDevice VulkanLogicalDevice::initialize(const VulkanPhysicalDevice &physicalDevice, const VkDeviceCreateInfo &deviceCreateInfo) const {
        VkDevice logicalDevice;
        if (physicalDevice.createDevice(deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device");
        }
        return logicalDevice;
    }

    VulkanLogicalDevice::VulkanLogicalDevice(const VulkanPhysicalDevice &physicalDevice, const VkDeviceCreateInfo &deviceCreateInfo) noexcept:
        _device(initialize(physicalDevice, deviceCreateInfo)),
        _physicalDevice(physicalDevice),
        _deviceCreateInfo(deviceCreateInfo) {
    }

    VulkanLogicalDevice::VulkanLogicalDevice(VulkanLogicalDevice &&other) noexcept :
        _device(other._device),
        _physicalDevice(other._physicalDevice),
        _deviceCreateInfo(other._deviceCreateInfo) {
        other._device = VK_NULL_HANDLE;
    }

    VulkanDeviceQueue VulkanLogicalDevice::getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) const {
        VkQueue queue;
        vkGetDeviceQueue(_device, queueFamilyIndex, queueIndex, &queue);
        return VulkanDeviceQueue(queue);
    }

    VkDeviceQueueCreateInfo const* VulkanLogicalDevice::getQueueCreateInfos(uint32_t *pCount) const {
        *pCount = _deviceCreateInfo.queueCreateInfoCount;
        return _deviceCreateInfo.pQueueCreateInfos;
    }


    VulkanLogicalDevice::~VulkanLogicalDevice() {
        if (_device != VK_NULL_HANDLE) {
            vkDestroyDevice(_device, nullptr);
        }
    }
} // Tutorial