//
// Created by 沖田大河 on 2026/02/14.
//

#ifndef TUTORIAL_REQUIRED_VULKAN_EXTENSIONS_PROVIDER_H
#define TUTORIAL_REQUIRED_VULKAN_EXTENSIONS_PROVIDER_H

#include <vector>
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#   include <vulkan/vulkan.h>
#else
import vulkan_hpp;
#endif

namespace Tutorial::Graphics {
    /**
     * 必須のVulkan拡張機能一覧のプロバイダ
     */
    class RequiredVulkanExtensionsProvider {
    public:
        RequiredVulkanExtensionsProvider() = default;

        /**
         * サポートされる必要のあるVulkan拡張機能の名称一覧を取得する
         * @returns サポートされる必要のあるVulkan拡張機能の名称一覧
         */
        [[nodiscard]] std::vector<const char*> getRequiredVulkanExtensionNames() const;

        /**
         * Vulkanインスタンスの作成に必要なフラグビットを取得する
         * @returns Vulkanインスタンスの作成に必要なフラグビット
         */
        [[nodiscard]] VkInstanceCreateFlagBits getRequiredVulkanInstanceCreateFlagBits() const;

        ~RequiredVulkanExtensionsProvider() = default;
    };
}
#endif //TUTORIAL_REQUIRED_VULKAN_EXTENSIONS_PROVIDER_H