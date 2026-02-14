//
// Created by 沖田大河 on 2026/02/14.
//

#include "VulkanClient.h"

#include <iostream>

namespace Tutorial::Graphics {

    vk::raii::Instance VulkanClient::instantiateVulkan() const {
        auto requiredExtensions = _requiredVulkanExtensionsProvider.getRequiredVulkanExtensionNames();
        const auto requiredExtensionCount = static_cast<uint32_t>(requiredExtensions.size());

        validateExtensions();

        vk::InstanceCreateInfo instanceCreateInfo {
            .flags = _requiredVulkanExtensionsProvider.getRequiredVulkanInstanceCreateFlagBits(),
            .pApplicationInfo = &_appInfo,
            .enabledExtensionCount = requiredExtensionCount,
            .ppEnabledExtensionNames = requiredExtensions.data(),
        };
        auto instance = vk::raii::Instance(_context, instanceCreateInfo);
        return instance;
    }

    void VulkanClient::validateExtensions() const {
        // サポートが必要となる拡張機能一覧を取得し、実際にサポートされている拡張機能に含まれているかを確認する
        auto requiredExtensions = _requiredVulkanExtensionsProvider.getRequiredVulkanExtensionNames();
        const auto requiredExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        auto actualExtensions = _context.enumerateInstanceExtensionProperties();
        for (uint32_t i = 0; i < requiredExtensionCount; i++) {
            // 必須の拡張機能がサポートされている拡張機能の中に存在しないなら例外をスローする
            auto requiredExtension = requiredExtensions[i];
            if (std::ranges::none_of(
                actualExtensions,
                [requiredExtension](const auto& actualExtension) { return strcmp(actualExtension.extensionName, requiredExtension) == 0;}
                )) {
                throw std::runtime_error("Required extension not supported: " + std::string(requiredExtension));
                };
            std::cout << "extension:" + std::string(requiredExtension) + " is supported" << std::endl;
        }
    }
}

