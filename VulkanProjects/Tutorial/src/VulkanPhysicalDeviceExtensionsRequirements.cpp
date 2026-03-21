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
        auto supportedDeviceExtensionProperties = Span<VkExtensionProperties>::stackAlloc(deviceExtensionCount);
        physicalDevice.enumerateExtensionProperties(nullptr, &deviceExtensionCount, supportedDeviceExtensionProperties.getHeadPtr());
        supportedDeviceExtensionProperties.markFilled();

        // サポートが必要なデバイス拡張機能すべてが、物理デバイスがサポートするデバイス拡張機能の中に存在するかを確認する
        for (const auto& requiredDeviceExtensionName : requiredDeviceExtensionNames) {
            if (!isRequiredDeviceExtensionSupported(supportedDeviceExtensionProperties, requiredDeviceExtensionName)) {
                return false;
            }
        }
        return true;
    }

    Span<VkExtensionProperties> VulkanPhysicalDeviceExtensionsRequirements::getDeviceExtensionProperties(const VulkanPhysicalDevice &physicalDevice) const {
        uint32_t deviceExtensionCount = 0;
        physicalDevice.enumerateExtensionProperties(nullptr, &deviceExtensionCount, nullptr);
        auto supportedDeviceExtensionProperties = Span<VkExtensionProperties>::stackAlloc(deviceExtensionCount);
        physicalDevice.enumerateExtensionProperties(nullptr, &deviceExtensionCount, supportedDeviceExtensionProperties.getHeadPtr());
        supportedDeviceExtensionProperties.markFilled();
        return supportedDeviceExtensionProperties;
    }

    bool VulkanPhysicalDeviceExtensionsRequirements::hasVK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME(const VulkanPhysicalDevice &physicalDevice) const {
        const auto deviceExtensionProperties = getDeviceExtensionProperties(physicalDevice);
        for (auto deviceExtensionProperty : deviceExtensionProperties) {
            if (strcmp(deviceExtensionProperty.extensionName, "VK_KHR_portability_subset") == 0) {
                return true;
            }
        }
        return false;
    }

    Span<char const *> VulkanPhysicalDeviceExtensionsRequirements::AsVkDeviceExtensionNames(const VulkanPhysicalDevice &physicalDevice) const {
        uint32_t requiredExtensionCount = std::size(requiredDeviceExtensionNames);
        if (hasVK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME(physicalDevice)) {
            ++requiredExtensionCount; // VK_KHR_portability_subsetの分を加える
        }
        auto result = Span<char const *>::stackAlloc(requiredExtensionCount);
        for (auto requiredDeviceExtensionName : requiredDeviceExtensionNames) {
            result.Add(requiredDeviceExtensionName);
        }
        if (hasVK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME(physicalDevice)) {
            result.Add("VK_KHR_portability_subset");
        }
        return result;
    }
} // Graphics
