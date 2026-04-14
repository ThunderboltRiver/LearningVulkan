//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_FORMAT_REQUIREMENTS_H
#define TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_FORMAT_REQUIREMENTS_H
#include "Span.h"
#include "VulkanSurface.h"

namespace Tutorial::Graphics {

    /**
     * SwapChainの作成に使用されるサーフェイスのフォーマットに対する要求を表すクラス
     * Vulkanのサーフェイスフォーマットに対する要求を表すクラス
     */
    class SwapChainVulkanSurfaceFormatRequirements {

    public:
        explicit SwapChainVulkanSurfaceFormatRequirements();

        /**
         * サーフェスのフォーマットがこの要求を満たすかを確認する
         * @param surfaceFormat サーフェスのフォーマット
         * @return surfaceFormatがこの要求を満たすならtrue、そうでないならfalse
         */
        [[nodiscard]] bool isSatisfiedBy(VkSurfaceFormatKHR surfaceFormat) const;

        /**
         * 物理デバイスがサポートするサーフェスのフォーマットの中から、この要求を満たすサーフェスのフォーマットを選択する
         * @param surfaceFormats 物理デバイスがサポートするサーフェスのフォーマットの配列
         * @return この要求を満たすサーフェスのフォーマット
         * @throws std::runtime_error この要求を満たすサーフェスのフォーマットが物理デバイスに存在しない場合
         */
        [[nodiscard]] VkSurfaceFormatKHR chooseSatisfiedOne(const Span<VkSurfaceFormatKHR> &surfaceFormats) const;
    };
}

#endif //TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_FORMAT_REQUIREMENTS_H