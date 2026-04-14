//
// Created by 沖田大河 on 2026/03/15.
//

#include <vulkan/vulkan.h>
#include "VulkanPhysicalDeviceExtensionsRequirements.h"

#include <cstring>
#include <iterator>

#include "Logger.h"

namespace Tutorial::Graphics {

    /**
     * このアプリケーションが物理デバイスに対して必要とするデバイス拡張機能の名前の配列
     */
    constexpr static char const* requiredDeviceExtensionNames[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    /**
     * 物理デバイスによっては必ず有効化する必要のあるデバイス拡張機能の名前の配列
     * この配列に含まれる拡張をサポートする物理デバイスは、必ずその拡張機能を有効化する必要がある
     */
    constexpr static char const* requiredDeviceExtensionNamesIfSupported[] = {
        "VK_KHR_portability_subset",
    };

    bool VulkanPhysicalDeviceExtensionsRequirements::isDeviceExtensionSupported(
    const Span<VkExtensionProperties>& supportedDeviceExtensions, const char *requiredExtensionName) const {
        for (const auto& supportedDeviceExtensionProperty : supportedDeviceExtensions) {
            if (strcmp(requiredExtensionName, supportedDeviceExtensionProperty.extensionName) == 0) {
                return true;
            }
        }
        return false;
    }

    uint32_t VulkanPhysicalDeviceExtensionsRequirements::getCountOfRequiredDeviceExtensionsIfSupported(const Span<VkExtensionProperties>& supportedDeviceExtensions) const {
        uint32_t count = 0;
        for (const auto& requiredDeviceExtensionName : requiredDeviceExtensionNamesIfSupported) {
            if (isDeviceExtensionSupported(supportedDeviceExtensions, requiredDeviceExtensionName)) {
                ++count;
            }
        }
        return count;
    }

    bool VulkanPhysicalDeviceExtensionsRequirements::isSatisfiedBy(const VulkanPhysicalDevice &physicalDevice) const {
        const auto supportedDeviceExtensionProperties = physicalDevice.enumerateExtensionProperties(nullptr);
        // サポートが必要なデバイス拡張機能すべてが、物理デバイスがサポートするデバイス拡張機能の中に存在するかを確認する
        for (const auto& requiredDeviceExtensionName : requiredDeviceExtensionNames) {
            if (!isDeviceExtensionSupported(supportedDeviceExtensionProperties, requiredDeviceExtensionName)) {
                return false;
            }
        }
        return true;
    }

    Span<char const *> VulkanPhysicalDeviceExtensionsRequirements::asVkDeviceExtensionNames(const VulkanPhysicalDevice &physicalDevice) const {
        const auto supportedDeviceExtensionProperties = physicalDevice.enumerateExtensionProperties(nullptr);

        // 物理デバイスが必ず有効化する必要のある拡張機能の中で、物理デバイスがサポートするものがあるなら、その分も加える
        const auto totalRequiredDeviceExtensionsCount = std::size(requiredDeviceExtensionNames) + getCountOfRequiredDeviceExtensionsIfSupported(supportedDeviceExtensionProperties);

        // 常に必須の拡張機能を追加
        auto result = Span<char const *>::stackAlloc(totalRequiredDeviceExtensionsCount);
        for (auto requiredDeviceExtensionName : requiredDeviceExtensionNames) {
            result.Add(requiredDeviceExtensionName);
        }

        // 物理デバイスによっては必ず有効化する必要のある拡張機能の中で、物理デバイスがサポートするものがあるなら、その分も加える
        for (const auto& requiredDeviceExtensionName : requiredDeviceExtensionNamesIfSupported) {
            if (isDeviceExtensionSupported(supportedDeviceExtensionProperties, requiredDeviceExtensionName)) {
                result.Add(requiredDeviceExtensionName);
            }
        }
        return result;
    }
} // Graphics
