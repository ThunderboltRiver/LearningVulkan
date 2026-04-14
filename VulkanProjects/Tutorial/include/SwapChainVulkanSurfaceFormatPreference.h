//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_FORMAT_PREFERENCE_H
#define TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_FORMAT_PREFERENCE_H
#include "Span.h"
#include "VulkanSurface.h"

namespace Tutorial::Graphics {

    /**
     * SwapChainの作成に使用されるサーフェイスのフォーマットに対する選好を表すクラス
     * Vulkanのサーフェイスフォーマットに対する選好を表すクラス
     */
    class SwapChainVulkanSurfaceFormatPreference {

    public:
        explicit SwapChainVulkanSurfaceFormatPreference();

        /**
         * サーフェスのフォーマットがこの選好を満たすかを確認する
         * @param surfaceFormat サーフェスのフォーマット
         * @return surfaceFormatがこの選好を満たすならtrue、そうでないならfalse
         */
        [[nodiscard]] bool isSatisfiedBy(VkSurfaceFormatKHR surfaceFormat) const;

        /**
         * 物理デバイスがサポートするサーフェスのフォーマットの中から、この選好を満たすサーフェスのフォーマットを選択する
         * @param surfaceFormats 物理デバイスがサポートするサーフェスのフォーマットの配列
         * @return この選好を満たすサーフェスのフォーマット
         * @throws std::runtime_error この選好を満たすサーフェスのフォーマットが物理デバイスに存在しない場合
         */
        [[nodiscard]] VkSurfaceFormatKHR chooseSatisfiedOne(const Span<VkSurfaceFormatKHR> &surfaceFormats) const;
    };
}

#endif //TUTORIAL_SWAP_CHAIN_VULKAN_SURFACE_FORMAT_PREFERENCE_H