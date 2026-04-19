//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_VULKAN_SWAP_CHAIN_H
#define TUTORIAL_VULKAN_SWAP_CHAIN_H

#include <vulkan/vulkan.h>

#include "Span.h"
#include "ResourceManagement/Borrowed.h"
#include "ResourceManagement/OwnerShip.h"

namespace Tutorial::Graphics {
    namespace rsm = Tutorial::ResourceManagement;

    /**
     * Vulkanのスワップチェーンを表すクラス
     */
    class VulkanSwapChain {
        rsm::OwnerShip<VkSwapchainKHR> _pSwapChain;
        rsm::Borrowed<VkDevice> _vkDevice;
        VkSurfaceFormatKHR _surfaceFormat;
        VkExtent2D _extent;

        [[nodiscard]] rsm::OwnerShip<VkSwapchainKHR> resourceAcquisition(rsm::Borrowed<VkDevice> vkDevice, const VkSwapchainCreateInfoKHR &createInfo) const;

        void cacheProperties(const VkSwapchainCreateInfoKHR &createInfo);

    public:
        explicit VulkanSwapChain(rsm::Borrowed<VkDevice> vkDevice, const VkSwapchainCreateInfoKHR& createInfo);

        [[nodiscard]] rsm::Borrowed<VkSwapchainKHR> getHandle() const;

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
