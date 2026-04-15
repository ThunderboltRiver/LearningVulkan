//
// Created by 沖田大河 on 2026/03/19.
//

#include "../include/VulkanLogicalDevice.h"

#include <stdexcept>
#include <new>

#include "Span.h"

namespace Tutorial::Graphics {
    VkDevice VulkanLogicalDevice::initialize(const VulkanPhysicalDevice &physicalDevice, const VkDeviceCreateInfo &deviceCreateInfo) const {
        VkDevice logicalDevice;
        if (physicalDevice.createDevice(deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device");
        }
        return logicalDevice;
    }

    VulkanLogicalDevice::VulkanLogicalDevice(const VulkanPhysicalDevice &physicalDevice, const VkDeviceCreateInfo &deviceCreateInfo):
        _device(initialize(physicalDevice, deviceCreateInfo)),
        _physicalDevice(physicalDevice),
        _deviceQueueCreateInfos(getQueueCreateInfosFromDeviceInfo(deviceCreateInfo)) {
    }

    VkDevice VulkanLogicalDevice::getHandle() const {
        return _device;
    }

    VulkanLogicalDevice::VulkanLogicalDevice(VulkanLogicalDevice &&other) noexcept:
        _device(other._device),
        _physicalDevice(other._physicalDevice),
        _deviceQueueCreateInfos(std::move(other._deviceQueueCreateInfos )) {
        other._device = VK_NULL_HANDLE;
    }

    VulkanDeviceQueue VulkanLogicalDevice::getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) const {
        VkQueue queue;
        vkGetDeviceQueue(_device, queueFamilyIndex, queueIndex, &queue);
        return VulkanDeviceQueue(queue);
    }

    Span<VkDeviceQueueCreateInfo const> VulkanLogicalDevice::getQueueCreateInfosFromDeviceInfo(const VkDeviceCreateInfo &deviceCreateInfo) const {
        auto result = Span<VkDeviceQueueCreateInfo const>::stackAlloc(deviceCreateInfo.queueCreateInfoCount);
        for (uint32_t i = 0; i < deviceCreateInfo.queueCreateInfoCount; ++i) {
            result.Add(deviceCreateInfo.pQueueCreateInfos[i]);
        }
        return result;
    }

    Span<uint32_t const> VulkanLogicalDevice::getQueueFamilyIndices() const {
        auto result = Span<uint32_t const>::stackAlloc(_deviceQueueCreateInfos.getMaxElementCount());
        for (uint32_t i = 0; i < _deviceQueueCreateInfos.getMaxElementCount(); ++i) {
            uint32_t queueFamilyIndex = _deviceQueueCreateInfos[i].queueFamilyIndex;
            result.Add(queueFamilyIndex);
        }
        return result;
    }

    VulkanLogicalDevice::~VulkanLogicalDevice() {
        if (_device != VK_NULL_HANDLE) {
            vkDestroyDevice(_device, nullptr);
        }
    }
} // Tutorial