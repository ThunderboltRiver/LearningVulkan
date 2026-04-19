//
// Created by 沖田大河 on 2026/03/19.
//

#ifndef TUTORIAL_VULKAN_LOGICAL_DEVICE_H
#define TUTORIAL_VULKAN_LOGICAL_DEVICE_H

#include <vulkan/vulkan.h>
#include <ResourceManagement/Borrowed.h>
#include <ResourceManagement/OwnerShip.h>
#include "Span.h"
#include "Graphics/VulkanDeviceQueue.h"
#include "Graphics/VulkanPhysicalDevice.h"
#include "ResourceManagement/Alias.h"

namespace Tutorial::Graphics {
    /**
     * Vulkanの論理デバイスを表すクラス
     */
    class VulkanLogicalDevice {
        rsm::OwnerShip<VkDevice> _device;
        Span<VulkanDeviceQueue const> _queues;

        [[nodiscard]] rsm::OwnerShip<VkDevice> resourceAcquisition(const VulkanPhysicalDevice& physicalDevice, const VkDeviceCreateInfo& deviceCreateInfo) const;
        [[nodiscard]] Span<VulkanDeviceQueue const> createVulkanDeviceQueues(const VkDeviceCreateInfo& deviceCreateInfo) const;

        [[nodiscard]] uint32_t getQueueCreateCount(const VkDeviceCreateInfo &deviceCreateInfo) const;

    public:
        explicit VulkanLogicalDevice(const VulkanPhysicalDevice& physicalDevice, const VkDeviceCreateInfo& deviceCreateInfo);

        [[nodiscard]] rsm::Borrowed<VkDevice> getHandle() const;

        [[nodiscard]] SpanView<VulkanDeviceQueue const> getQueues() const;

        // コピー禁止。論理デバイスの所有権を持つのは一つのインスタンスのみにするため
        VulkanLogicalDevice(const VulkanLogicalDevice&) = delete;
        VulkanLogicalDevice& operator=(const VulkanLogicalDevice&) = delete;

        // ムーブコンストラクタ
        VulkanLogicalDevice(VulkanLogicalDevice&& other) noexcept;

        [[nodiscard]] VulkanDeviceQueue getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) const;

        [[nodiscard]] Span<uint32_t const> getQueueFamilyIndices() const;

        ~VulkanLogicalDevice();
    };
}

#endif //TUTORIAL_VULKAN_LOGICAL_DEVICE_H
