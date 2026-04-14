//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_VULKAN_SWAP_CHAIN_H
#define TUTORIAL_VULKAN_SWAP_CHAIN_H

#include "VulkanLogicalDevice.h"

namespace Tutorial::Graphics {

    /**
     * Vulkanのスワップチェーンを表すクラス
     */
    class VulkanSwapChain {
        VkSwapchainKHR _handle;
        VkDevice _logicalDeviceHandle;

        [[nodiscard]] VkSwapchainKHR acquisitionSwapChainResource(const VkSwapchainCreateInfoKHR &createInfo, const VulkanLogicalDevice &logicalDevice) const;

    public:
        explicit VulkanSwapChain(const VkSwapchainCreateInfoKHR& createInfo, const VulkanLogicalDevice& physicalDevice);

        [[nodiscard]] VkSwapchainKHR getHandle() const;

        // コピー禁止。スワップチェーンの所有権を持つのは一つのインスタンスのみにするため
        VulkanSwapChain(const VulkanSwapChain&) = delete;
        VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;

        // ムーブコンストラクタ
        VulkanSwapChain(VulkanSwapChain&& other) noexcept;

        // ムーブ代入演算子
        VulkanSwapChain& operator=(VulkanSwapChain&& other) noexcept;

        ~VulkanSwapChain();
    };
}


#endif //TUTORIAL_VULKAN_SWAP_CHAIN_H