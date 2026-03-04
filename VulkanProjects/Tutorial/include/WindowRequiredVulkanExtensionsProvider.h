//
// Created by 沖田大河 on 2026/03/02.
//

#ifndef TUTORIAL_WINDOWREQUIREDVULKANEXTENSIONSPROVIDER_H
#define TUTORIAL_WINDOWREQUIREDVULKANEXTENSIONSPROVIDER_H

#include "IRequiredVulkanExtensionsProvider.h"

namespace Tutorial::WindowHelper {
    // GLFWが要求するVulkan拡張機能のプロバイダ
    class WindowRequiredVulkanExtensionsProvider : public Graphics::IRequiredVulkanExtensionsProvider {
    public:
        WindowRequiredVulkanExtensionsProvider() = default;

        [[nodiscard]] uint32_t getRequiredInstanceExtensionCount() const override;

        void getRequiredInstanceExtensionNames(const Span<char const*>& result) const override;

        [[nodiscard]] VkInstanceCreateFlags getRequiredVulkanInstanceCreateFlagBits() const override;
    };
}

#endif //TUTORIAL_WINDOWREQUIREDVULKANEXTENSIONSPROVIDER_H