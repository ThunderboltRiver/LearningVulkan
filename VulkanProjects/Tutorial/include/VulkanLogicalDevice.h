//
// Created by 沖田大河 on 2026/03/19.
//

#ifndef TUTORIAL_VULKAN_LOGICAL_DEVICE_H
#define TUTORIAL_VULKAN_LOGICAL_DEVICE_H
#include <stdexcept>

#include "Span.h"
#include "VulkanDeviceQueue.h"
#include "VulkanPhysicalDevice.h"

namespace Tutorial::Graphics {
    class VulkanPhysicalDevice;

    /**
     * Vulkanの論理デバイスを表すクラス
     */
    class VulkanLogicalDevice {
        VkDevice _device = VK_NULL_HANDLE;
        const VulkanPhysicalDevice _physicalDevice;
        Span<VkDeviceQueueCreateInfo const> _deviceQueueCreateInfos;

        [[nodiscard]] VkDevice initialize(const VulkanPhysicalDevice& physicalDevice, const VkDeviceCreateInfo& deviceCreateInfo) const;

        [[nodiscard]] Span<VkDeviceQueueCreateInfo const> getQueueCreateInfosFromDeviceInfo(const VkDeviceCreateInfo &deviceCreateInfo) const;

    public:
        explicit VulkanLogicalDevice(const VulkanPhysicalDevice& physicalDevice, const VkDeviceCreateInfo& deviceCreateInfo);

        [[nodiscard]] VkDevice getHandle() const;

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