//
// Created by 沖田大河 on 2026/02/14.
//

#include "VulkanClient.h"
#include "Logger.h"

#include <iostream>

#include "Span.h"

namespace Tutorial::Graphics {

    VkInstance VulkanClient::instantiateVulkan() const {
        const auto requiredExtensionCount = _requiredVulkanExtensionsProvider.getRequiredInstanceExtensionCount();
        const auto requiredExtensions = Span<char const*>::stackAlloc(requiredExtensionCount);
        _requiredVulkanExtensionsProvider.getRequiredInstanceExtensionNames(requiredExtensions);

        uint32_t requiredLayerCount = 0;
        const auto requiredLayers = getRequiredLayers(&requiredLayerCount);

        validateRequiredLayer(requiredLayers, requiredLayerCount);
        validateRequiredExtensions(requiredExtensions);

        const VkInstanceCreateInfo instanceCreateInfo {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .flags = _requiredVulkanExtensionsProvider.getRequiredVulkanInstanceCreateFlagBits(),
            .pApplicationInfo = &_appInfo,
            .enabledLayerCount = requiredLayerCount,
            .ppEnabledLayerNames = requiredLayers,
            .enabledExtensionCount = requiredExtensionCount,
            .ppEnabledExtensionNames = requiredExtensions.headPtr,
        };
        VkInstance instance;
        if (const auto result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance: " + std::to_string(result));
        }
        return instance;
    }

    char const* const* VulkanClient::getRequiredLayers(uint32_t *pCount) const {
       if constexpr (!enableValidationLayers) {
            *pCount = 0;
            return nullptr;
        }
        *pCount = std::size(validationLayerNames);
        return validationLayerNames;
    }

    void VulkanClient::validateRequiredLayer(char const* const* requiredLayers, uint32_t count) const {
        if (requiredLayers == nullptr || count == 0) {
            return;
        }
        uint32_t supportedLayerCount = 0;
        // 実際にサポートされているレイヤー一覧を取得して、必須のレイヤーがサポートされているかを確認する
        if (vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr) != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance layers");
        }
        const auto supportedLayers = Span<VkLayerProperties>::stackAlloc(supportedLayerCount);
        if (vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.headPtr) != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance layers");
        }
        // 必須のレイヤーがサポートされているレイヤーの中に存在しないなら例外をスローする
        for (uint32_t i = 0; i < count; ++i) {
            if (const auto requiredLayerName = requiredLayers[i]; !isLayerSupported(requiredLayerName, supportedLayers)) {
                throw std::runtime_error("Required Vulkan validation layer not supported: " + std::string(requiredLayerName));
            }
        }
    }

    bool VulkanClient::isLayerSupported(const char* layerName, const Span<VkLayerProperties>& actualSupportedLayers) const {
        for (uint32_t i = 0; i < actualSupportedLayers.maxElementCount; ++i) {
            if (const auto actualSupportedLayer = actualSupportedLayers[i]; strcmp(actualSupportedLayer.layerName, layerName) == 0) {
                Debug::Logger::log("layer:" + std::string(layerName) + " is supported");
                return true;
            }
        }
        return false;
    }

    void VulkanClient::validateRequiredExtensions(const Span<char const*>& requiredExtensions) const {
        // 実際にサポートされている拡張機能一覧を取得して、必須の拡張機能がサポートされているかを確認する
        uint32_t supportedExtensionCount = getSupportedExtensionCount();
        const auto supportedExtensions = Span<VkExtensionProperties>::stackAlloc(supportedExtensionCount);
        if (const auto result = vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensions.headPtr); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance extensions: " + std::to_string(result));
        }
        // 必須の拡張機能がサポートされている拡張機能の中に存在しないなら例外をスローする
        for (uint32_t i = 0; i < requiredExtensions.maxElementCount; ++i) {
            const auto requiredExtension = requiredExtensions[i];
            if (!isExtensionSupported(requiredExtension, supportedExtensions)) {
                throw std::runtime_error("Required Vulkan extension not supported: " + std::string(requiredExtension));
            }
        }
    }

    uint32_t VulkanClient::getSupportedExtensionCount() const {
        uint32_t extensionCount = 0;
        if (const auto result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance extensions: " + std::to_string(result));
        }
        return extensionCount;
    }

    bool VulkanClient::isExtensionSupported(const char* extensionName, const Span<VkExtensionProperties>& actualSupportedExtensions) const {
        for (uint32_t i = 0; i < actualSupportedExtensions.maxElementCount; ++i) {
            if (const auto actualSupportedExtension = actualSupportedExtensions[i]; strcmp(actualSupportedExtension.extensionName, extensionName) == 0) {
                Debug::Logger::log("extension:" + std::string(extensionName) + " is supported");
                return true;
            }
        }
        return false;
    }

    VulkanClient::~VulkanClient() {
        vkDestroyInstance(_instance, nullptr);
    }
}

