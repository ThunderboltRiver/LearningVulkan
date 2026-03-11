//
// Created by 沖田大河 on 2026/02/14.
//

#ifndef TUTORIAL_VULKAN_CLIENT_H
#define TUTORIAL_VULKAN_CLIENT_H

#include "IRequiredVulkanExtensionsProvider.h"
#include "Span.h"
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
# include <vulkan/vulkan.h>
#else
import vulkan_hpp;
#endif

namespace Tutorial::Graphics {
    class VulkanClient {
        const VkApplicationInfo& _appInfo;
        const IRequiredVulkanExtensionsProvider& _requiredVulkanExtensionsProvider;
        VkInstance _instance;
#ifndef NDEBUG
        VkDebugUtilsMessengerEXT _debugMessenger{};
#endif

        [[nodiscard]] VkInstance instantiateVulkan() const;
        void validateRequiredExtensions(const Span<char const *> &requiredExtensions) const;

        [[nodiscard]] uint32_t getSupportedExtensionCount() const;

        bool isExtensionSupported(const char *extensionName, const Span<VkExtensionProperties> &actualSupportedExtensions) const;

#ifndef NDEBUG
        [[nodiscard]] bool checkValidationLayerSupport() const;
        void setupDebugMessenger();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
#endif

    public:
        VulkanClient(const VkApplicationInfo& appInfo, const IRequiredVulkanExtensionsProvider& requiredVulkanExtensionsProvider):
        _appInfo(appInfo),
        _requiredVulkanExtensionsProvider(requiredVulkanExtensionsProvider),
        _instance(instantiateVulkan()) {
#ifndef NDEBUG
            setupDebugMessenger();
#endif
        }

        // vkInstanceの所有権を持つのは一つのインスタンスのみにするためコピー禁止
        VulkanClient(const VulkanClient&) = delete;
        VulkanClient& operator=(const VulkanClient&) = delete;

        ~VulkanClient();
    };

}
#endif //TUTORIAL_VULKAN_CLIENT_H