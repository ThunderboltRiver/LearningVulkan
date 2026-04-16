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

using Tutorial::ResourceManagement::OwnerShip;
using Tutorial::ResourceManagement::Borrowed;

namespace Tutorial::Graphics {

    /**
     * Vulkanの論理デバイスを表すクラス
     */
    class VulkanLogicalDevice {
        OwnerShip<VkDevice> _device;
        const VulkanPhysicalDevice _physicalDevice;
        Span<VkDeviceQueueCreateInfo const> _deviceQueueCreateInfos;

        [[nodiscard]] OwnerShip<VkDevice> resourceAcquisition(const VulkanPhysicalDevice& physicalDevice, const VkDeviceCreateInfo& deviceCreateInfo) const;

        [[nodiscard]] Span<VkDeviceQueueCreateInfo const> getQueueCreateInfosFromDeviceInfo(const VkDeviceCreateInfo &deviceCreateInfo) const;

    public:
        explicit VulkanLogicalDevice(const VulkanPhysicalDevice& physicalDevice, const VkDeviceCreateInfo& deviceCreateInfo);

        [[nodiscard]] Borrowed<VkDevice> getHandle() const;

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