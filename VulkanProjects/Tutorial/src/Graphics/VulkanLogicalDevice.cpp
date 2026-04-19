//
// Created by 沖田大河 on 2026/03/19.
//

#include "Graphics/VulkanLogicalDevice.h"

#include <stdexcept>

#include "Span.h"
#include "SpanView.h"

namespace Tutorial::Graphics {
    namespace rsm = Tutorial::ResourceManagement;

    rsm::OwnerShip<VkDevice> VulkanLogicalDevice::resourceAcquisition(const VulkanPhysicalDevice &physicalDevice, const VkDeviceCreateInfo &deviceCreateInfo) const {
        VkDevice logicalDevice;
        if (physicalDevice.createDevice(deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device");
        }
        return rsm::OwnerShip(logicalDevice);
    }

    Span<VulkanDeviceQueue const> VulkanLogicalDevice::createVulkanDeviceQueues(const VkDeviceCreateInfo &deviceCreateInfo) const {
        auto result = Span<VulkanDeviceQueue const>::stackAlloc(getQueueCreateCount(deviceCreateInfo));
        for (uint32_t i = 0; i < deviceCreateInfo.queueCreateInfoCount; ++i) {
            const auto queueCreateInfo = deviceCreateInfo.pQueueCreateInfos[i];
             for (uint32_t queueIndex = 0; queueIndex < queueCreateInfo.queueCount; ++queueIndex) {
                 // キューファミリーインデックスとキューインデックスを組み合わせて、キューのハンドルを取得する
                 VkQueue queue;
                 vkGetDeviceQueue(_device.getRawHandle(), queueCreateInfo.queueFamilyIndex, queueIndex, &queue);
                 result.Add(VulkanDeviceQueue(rsm::Borrowed(queue), queueIndex, queueCreateInfo.queueFamilyIndex));
             }
        }
        return result;
    }

    uint32_t VulkanLogicalDevice::getQueueCreateCount(const VkDeviceCreateInfo &deviceCreateInfo) const {
        uint32_t result = 0;
        for (uint32_t i = 0; i < deviceCreateInfo.queueCreateInfoCount; ++i) {
            const auto queueCreateInfo = deviceCreateInfo.pQueueCreateInfos[i];
            result += queueCreateInfo.queueCount;
        }
        return result;
    }

    VulkanLogicalDevice::VulkanLogicalDevice(const VulkanPhysicalDevice &physicalDevice, const VkDeviceCreateInfo &deviceCreateInfo):
        _device(resourceAcquisition(physicalDevice, deviceCreateInfo)),
        _queues(createVulkanDeviceQueues(deviceCreateInfo)) {
    }

    rsm::Borrowed<VkDevice> VulkanLogicalDevice::getHandle() const {
        return _device.borrow();
    }

    SpanView<VulkanDeviceQueue const> VulkanLogicalDevice::getQueues() const { return _queues.asView(); }

    VulkanLogicalDevice::VulkanLogicalDevice(VulkanLogicalDevice &&other) noexcept:
        _device(other._device.move()),
        _queues(std::move(other._queues))
    {
        other._device = rsm::OwnerShip<VkDevice>::MOVED();
    }

    VulkanDeviceQueue VulkanLogicalDevice::getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) const {
        for (auto& queue : _queues) {
            if (queue.getQueueFamilyIndex() == queueFamilyIndex && queue.getQueueIndex() == queueIndex) {
                return queue;
            }
        }
        throw std::runtime_error("Failed to find a queue with the specified family index and queue index");
    }

    Span<uint32_t const> VulkanLogicalDevice::getQueueFamilyIndices() const {
        Span<uint32_t const> result = Span<uint32_t const>::stackAlloc(_queues.getElementCount());
        for (auto& queue : _queues) {
            result.Add(queue.getQueueFamilyIndex());
        }
        return result;
    }

    VulkanLogicalDevice::~VulkanLogicalDevice() {
        if (_device.isNotMoved())
            vkDestroyDevice(_device.getRawHandle(), nullptr);
        }
}
