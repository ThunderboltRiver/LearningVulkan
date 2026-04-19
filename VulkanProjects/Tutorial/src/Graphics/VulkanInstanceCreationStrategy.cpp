//
// Created by 沖田大河 on 2026/04/18.
//

#include "Graphics/VulkanInstanceCreationStrategy.h"
#include "Debug/Logger.h"
#include <cstring>
#include <string>

namespace Tutorial::Graphics {
    VulkanInstanceCreationStrategy::VulkanInstanceCreationStrategy(
        const IRequiredVulkanExtensionsProvider &requiredVulkanExtensionsProvider,
        const RequiredVulkanInstanceLayerProvider &requiredVulkanInstanceLayerProvider):
        _requiredVulkanExtensionsProvider(requiredVulkanExtensionsProvider),
        _requiredVulkanInstanceLayerProvider(requiredVulkanInstanceLayerProvider) {
    }

    VulkanInstance VulkanInstanceCreationStrategy::createVulkanInstance(const VkApplicationInfo &applicationInfo) const {

        const auto requiredExtensionCount = _requiredVulkanExtensionsProvider.getRequiredInstanceExtensionCount();
        auto requiredExtensions = Span<char const*>::stackAlloc(requiredExtensionCount);
        _requiredVulkanExtensionsProvider.getRequiredInstanceExtensionNames(requiredExtensions);

        const auto requiredLayerNames = _requiredVulkanInstanceLayerProvider.getLayerNames();

        validateRequiredLayer(requiredLayerNames);
        validateRequiredExtensions(requiredExtensions);

        const VkInstanceCreateInfo instanceCreateInfo {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .flags = _requiredVulkanExtensionsProvider.getRequiredVulkanInstanceCreateFlagBits(),
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = requiredLayerNames.getElementCount(),
            .ppEnabledLayerNames = requiredLayerNames.getHeadPtr(),
            .enabledExtensionCount = requiredExtensionCount,
            .ppEnabledExtensionNames = requiredExtensions.getHeadPtr(),
        };
        return VulkanInstance(instanceCreateInfo);
    }

    void VulkanInstanceCreationStrategy::validateRequiredLayer(const Span<char const*>& requiredLayerNames) const {
        if (requiredLayerNames.getMaxElementCount() == 0) {
            // 必須のレイヤーがない場合は、サポートされているレイヤーを確認する必要はないので、ここで終了する
            return;
        }
        uint32_t supportedLayerCount = 0;
        // 実際にサポートされているレイヤー一覧を取得して、必須のレイヤーがサポートされているかを確認する
        if (vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr) != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance layers");
        }
        auto supportedLayers = Span<VkLayerProperties>::stackAlloc(supportedLayerCount);
        if (vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.getHeadPtr()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance layers");
        }
        supportedLayers.markFilled();
        // 必須のレイヤーがサポートされているレイヤーの中に存在しないなら例外をスローする
        for (auto requiredLayerName : requiredLayerNames) {
            if (!isLayerSupported(requiredLayerName, supportedLayers)) {
                throw std::runtime_error("Required Vulkan validation layer not supported: " + std::string(requiredLayerName));
            }
        }
    }

    bool VulkanInstanceCreationStrategy::isLayerSupported(const char* layerName, const Span<VkLayerProperties>& actualSupportedLayers) const {
        for (uint32_t i = 0; i < actualSupportedLayers.getMaxElementCount(); ++i) {
            if (const auto actualSupportedLayer = actualSupportedLayers[i]; strcmp(actualSupportedLayer.layerName, layerName) == 0) {
                Debug::Logger::log("layer:" + std::string(layerName) + " is supported");
                return true;
            }
        }
        return false;
    }

    void VulkanInstanceCreationStrategy::validateRequiredExtensions(const Span<char const*>& requiredExtensions) const {
        // 実際にサポートされている拡張機能一覧を取得して、必須の拡張機能がサポートされているかを確認する
        uint32_t supportedExtensionCount = getSupportedExtensionCount();
        auto supportedExtensions = Span<VkExtensionProperties>::stackAlloc(supportedExtensionCount);
        if (const auto result = vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensions.getHeadPtr()); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance extensions: " + std::to_string(result));
        }
        supportedExtensions.markFilled();
        // 必須の拡張機能がサポートされている拡張機能の中に存在しないなら例外をスローする
        for (uint32_t i = 0; i < requiredExtensions.getMaxElementCount(); ++i) {
            const auto requiredExtension = requiredExtensions[i];
            if (!isExtensionSupported(requiredExtension, supportedExtensions)) {
                throw std::runtime_error("Required Vulkan extension not supported: " + std::string(requiredExtension));
            }
        }
    }

    uint32_t VulkanInstanceCreationStrategy::getSupportedExtensionCount() const {
        uint32_t extensionCount = 0;
        if (const auto result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance extensions: " + std::to_string(result));
        }
        return extensionCount;
    }

    bool VulkanInstanceCreationStrategy::isExtensionSupported(const char* extensionName, const Span<VkExtensionProperties>& actualSupportedExtensions) const {
        for (uint32_t i = 0; i < actualSupportedExtensions.getMaxElementCount(); ++i) {
            if (const auto actualSupportedExtension = actualSupportedExtensions[i]; strcmp(actualSupportedExtension.extensionName, extensionName) == 0) {
                Debug::Logger::log("extension:" + std::string(extensionName) + " is supported");
                return true;
            }
        }
        return false;
    }

}
