//
// Created by 沖田大河 on 2026/04/12.
//

#include "SwapChainImageCountSelectStrategy.h"

#include <algorithm>

namespace Tutorial::Graphics {
    SwapChainImageCountSelectStrategy::SwapChainImageCountSelectStrategy() = default;

    uint32_t SwapChainImageCountSelectStrategy::chooseImageCount(const VkSurfaceCapabilitiesKHR &surfaceCapabilities) const {
        const auto maxImageCount = surfaceCapabilities.maxImageCount;
        const auto minImageCount = surfaceCapabilities.minImageCount;
        if (maxImageCount == 0) {
            // 最大が0の場合は制限なしを意味するので、最小枚数とデフォルト枚数のうち大きい方を選択する
            return std::max(minImageCount, DEFAULT_IMAGE_COUNT);
        }
        if (minImageCount == maxImageCount) {
            // 最小枚数と最大枚数が同じ場合は、その枚数を選択する
            return maxImageCount;
        }
        // 最小枚数と最大枚数の範囲内で、デフォルト枚数に最も近い枚数を選択する
        return std::min(std::max(minImageCount, DEFAULT_IMAGE_COUNT), maxImageCount);
    }
} // Graphics
