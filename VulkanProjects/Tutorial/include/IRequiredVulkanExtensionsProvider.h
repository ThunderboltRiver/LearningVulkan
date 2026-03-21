//
// Created by 沖田大河 on 2026/02/14.
//

#ifndef TUTORIAL_REQUIRED_VULKAN_EXTENSIONS_PROVIDER_H
#define TUTORIAL_REQUIRED_VULKAN_EXTENSIONS_PROVIDER_H

#include "Span.h"
#include <vulkan/vulkan.h>

namespace Tutorial::Graphics {
    /**
     * 必須のVulkan拡張機能一覧のプロバイダinterface
     */
    class IRequiredVulkanExtensionsProvider {

    public:
        virtual ~IRequiredVulkanExtensionsProvider() = default;

        /**
         * サポートされる必要のあるInstance-拡張機能の数を取得する
         * @returns サポートされる必要のあるInstance-拡張機能の数
         */
        [[nodiscard]] virtual uint32_t getRequiredInstanceExtensionCount() const = 0;

        /**
         * サポートされる必要のあるInstance-拡張機能の名称一覧を取得する
         * @param result サポートされる必要のあるInstance-拡張機能の名称を格納するためのchar const*型の配列へのSpan
         */
        virtual void getRequiredInstanceExtensionNames(Span<char const*>& result) const = 0;

        /**
         * Vulkanインスタンスの作成に必要なフラグビットを取得する
         * @returns Vulkanインスタンスの作成に必要なフラグビット
         */
        [[nodiscard]] virtual VkInstanceCreateFlags getRequiredVulkanInstanceCreateFlagBits() const = 0;
    };
}
#endif //TUTORIAL_REQUIRED_VULKAN_EXTENSIONS_PROVIDER_H