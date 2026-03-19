//
// Created by 沖田大河 on 2026/03/15.
//

#include "VulkanPhysicalDeviceExtensionsRequirements.h"

#include <cstring>
#include <iterator>

namespace Tutorial::Graphics {

    constexpr static char const* requiredDeviceExtensionNames[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    bool VulkanPhysicalDeviceExtensionsRequirements::isRequiredDeviceExtensionSupported(
    const Span<VkExtensionProperties>& supportedDeviceExtensions, const char *requiredExtensionName) const {
        for (const auto& supportedDeviceExtensionProperty : supportedDeviceExtensions) {
            if (strcmp(requiredExtensionName, supportedDeviceExtensionProperty.extensionName) == 0) {
                return true;
            }
        }
        return false;
    }

    bool VulkanPhysicalDeviceExtensionsRequirements::isSatisfiedBy(const VulkanPhysicalDevice &physicalDevice) const {
        // 実際に物理デバイスがサポートするデバイス拡張機能の配列を取得する
        uint32_t deviceExtensionCount = 0;
        physicalDevice.enumerateExtensionProperties(nullptr, &deviceExtensionCount, nullptr);
        const auto supportedDeviceExtensionProperties = Span<VkExtensionProperties>::stackAlloc(deviceExtensionCount);
        physicalDevice.enumerateExtensionProperties(nullptr, &deviceExtensionCount, supportedDeviceExtensionProperties.headPtr);

        // サポートが必要なデバイス拡張機能すべてが、物理デバイスがサポートするデバイス拡張機能の中に存在するかを確認する
        for (const auto& requiredDeviceExtensionName : requiredDeviceExtensionNames) {
            if (!isRequiredDeviceExtensionSupported(supportedDeviceExtensionProperties, requiredDeviceExtensionName)) {
                return false;
            }
        }
    }

    const Span<char const *> & VulkanPhysicalDeviceExtensionsRequirements::AsVkDeviceExtensionNames() const {
        uint32_t deviceExtensionCount = std::size(requiredDeviceExtensionNames);
        const auto& result = Span<char const *>::stackAlloc(deviceExtensionCount);
        uint32_t index = 0;
        for (auto& extensionName : result) {
            extensionName = requiredDeviceExtensionNames[index];
            ++index;
        }
        return result;
    }
} // Graphics
