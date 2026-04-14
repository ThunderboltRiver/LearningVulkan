//
// Created by 沖田大河 on 2026/04/11.
//

#include "SwapChainPresentModePreference.h"

namespace Tutorial::Graphics {

    SwapChainPresentModePreference::SwapChainPresentModePreference() = default;

    VkPresentModeKHR SwapChainPresentModePreference::chooseSatisfiedOne(const Span<VkPresentModeKHR> &supportedPresentModes) const {
        auto isExistsFifoPresentMode = false;
        for (const auto& supportedPresentMode : supportedPresentModes) {
            if (supportedPresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                // 要件を満たすプレゼントモードが存在する場合はそれを選択する
                return VK_PRESENT_MODE_MAILBOX_KHR;
            }
            isExistsFifoPresentMode |= supportedPresentMode == VK_PRESENT_MODE_FIFO_KHR;
        }
        if (isExistsFifoPresentMode) {
            // VK_PRESENT_MODE_FIFO_KHRが利用可能であればそれを選択する
            return VK_PRESENT_MODE_FIFO_KHR;
        }
        throw std::runtime_error("Failed to find a present mode that satisfies the requirements");
    }
}
