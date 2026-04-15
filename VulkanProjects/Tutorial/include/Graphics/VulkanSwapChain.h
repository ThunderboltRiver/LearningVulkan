//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_VULKAN_SWAP_CHAIN_H
#define TUTORIAL_VULKAN_SWAP_CHAIN_H

#include "Graphics/VulkanLogicalDevice.h"

namespace Tutorial::Graphics {

    /**
     * Vulkanのスワップチェーンを表すクラス
     */
    class VulkanSwapChain {
        VkSwapchainKHR _handle;
        VkDevice _vkDevice;
        VkSurfaceFormatKHR _surfaceFormat;
        VkExtent2D _extent;

        [[nodiscard]] VkSwapchainKHR acquisitionSwapChainResource(VkDevice vkDevice, const VkSwapchainCreateInfoKHR &createInfo) const;

        void cacheProperties(const VkSwapchainCreateInfoKHR &createInfo);

    public:
        explicit VulkanSwapChain(VkDevice vkDevice, const VkSwapchainCreateInfoKHR& createInfo);

        [[nodiscard]] VkSwapchainKHR getHandle() const;

        /**
         * vkGetSwapchainImagesKHR のラッパー。スワップチェーンが所有する画像の配列を返す
         * @return スワップチェーンが所有する画像の配列
         */
        [[nodiscard]] Span<VkImage> getImages() const;

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