//
// Created by 沖田大河 on 2026/02/14.
//

#include "VulkanClient.h"
#include "Logger.h"

#include <iostream>

namespace Tutorial::Graphics {

    VkInstance VulkanClient::instantiateVulkan() const {
        auto requiredExtensions = _requiredVulkanExtensionsProvider.getRequiredVulkanExtensionNames();
        const auto requiredExtensionCount = static_cast<uint32_t>(requiredExtensions.size());

        validateRequiredExtensions(requiredExtensions.data(), requiredExtensionCount);

        const VkInstanceCreateInfo instanceCreateInfo {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .flags = _requiredVulkanExtensionsProvider.getRequiredVulkanInstanceCreateFlagBits(),
            .pApplicationInfo = &_appInfo,
            .enabledExtensionCount = requiredExtensionCount,
            .ppEnabledExtensionNames = requiredExtensions.data(),
        };
        VkInstance instance;
        if (const auto result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance: " + std::to_string(result));
        }
        return instance;
    }

    void VulkanClient::validateRequiredExtensions(const char* const* requiredExtensions, const uint32_t requiredExtensionCount) const {
        // 実際にサポートされている拡張機能一覧を取得して、必須の拡張機能がサポートされているかを確認する
        uint32_t supportedExtensionCount = getSupportedExtensionCount();
        VkExtensionProperties supportedExtensions[supportedExtensionCount]; // TODO: これはC++20のVLA。自作の動的配列クラスに置き換えるべき
        if (const auto result = vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensions); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance extensions: " + std::to_string(result));
        }
        // 必須の拡張機能がサポートされている拡張機能の中に存在しないなら例外をスローする
        for (uint32_t i = 0; i < requiredExtensionCount; ++i) {
            const auto requiredExtension = requiredExtensions[i];
            if (!isExtensionSupported(requiredExtension, supportedExtensions, supportedExtensionCount)) {
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

    bool VulkanClient::isExtensionSupported(const char* extensionName, const VkExtensionProperties* actualSupportedExtensions, const uint32_t extensionsCount) const {
        for (uint32_t i = 0; i < extensionsCount; ++i) {
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

