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
        constexpr static char const* validationLayerNames[] = { "VK_LAYER_KHRONOS_validation" };
#ifndef NDEBUG
        constexpr static bool enableValidationLayers = true;
#elif
        constexpr static bool enableValidationLayers = false;
#endif
        VkInstance _instance;

        [[nodiscard]] VkInstance instantiateVulkan() const;

        char const* const* getRequiredLayers(uint32_t *pCount) const;

        void validateRequiredLayer(char const* const* requiredLayers, uint32_t count) const;

        bool isLayerSupported(const char *layerName, const Span<VkLayerProperties> &actualSupportedLayers) const;

        void validateRequiredExtensions(const Span<char const *> &requiredExtensions) const;

        [[nodiscard]] uint32_t getSupportedExtensionCount() const;

        bool isExtensionSupported(const char *extensionName, const Span<VkExtensionProperties> &actualSupportedExtensions) const;

    public:
        VulkanClient(const VkApplicationInfo& appInfo, const IRequiredVulkanExtensionsProvider& requiredVulkanExtensionsProvider):
        _appInfo(appInfo),
        _requiredVulkanExtensionsProvider(requiredVulkanExtensionsProvider),
        _instance(instantiateVulkan()) {}

        // vkInstanceの所有権を持つのは一つのインスタンスのみにするためコピー禁止
        VulkanClient(const VulkanClient&) = delete;
        VulkanClient& operator=(const VulkanClient&) = delete;

        ~VulkanClient();
    };

}
#endif //TUTORIAL_VULKAN_CLIENT_H