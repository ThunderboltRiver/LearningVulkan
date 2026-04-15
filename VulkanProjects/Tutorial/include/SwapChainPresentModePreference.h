//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_SWAP_CHAIN_PRESENT_MODE_PREFERENCE_H
#define TUTORIAL_SWAP_CHAIN_PRESENT_MODE_PREFERENCE_H

#include <Span.h>
#include <vulkan/vulkan.h>

namespace Tutorial::Graphics {

    /**
     * スワップチェーンのプレゼントモードの要件を表すクラス
     */
    class SwapChainPresentModePreference {

    public:
        explicit SwapChainPresentModePreference();

        /**
         * 物理デバイスがサポートするプレゼントモードの中から、この要件を満たすプレゼントモードを選択する
         * @param supportedPresentModes 物理デバイスがサポートするプレゼントモードの配列
         * @return この要件を満たすプレゼントモード
         * @throws std::runtime_error この要件を満たすプレゼントモードが物理デバイスに存在しない場合
         */
        [[nodiscard]] VkPresentModeKHR chooseSatisfiedOne(const Span<VkPresentModeKHR>& supportedPresentModes) const;
    };
}
#endif //TUTORIAL_SWAP_CHAIN_PRESENT_MODE_PREFERENCE_H