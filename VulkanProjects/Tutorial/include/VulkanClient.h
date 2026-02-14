//
// Created by 沖田大河 on 2026/02/14.
//

#ifndef TUTORIAL_VULKAN_CLIENT_H
#define TUTORIAL_VULKAN_CLIENT_H

#include "RequiredVulkanExtensionsProvider.h"
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
# include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

namespace Tutorial::Graphics {
    class VulkanClient {
        const vk::ApplicationInfo& _appInfo;
        const RequiredVulkanExtensionsProvider& _requiredVulkanExtensionsProvider;
        const vk::raii::Context _context;
        const vk::raii::Instance _instance;

        vk::raii::Instance instantiateVulkan() const;
        void validateExtensions() const;

    public:
        VulkanClient(const vk::ApplicationInfo& appInfo, const RequiredVulkanExtensionsProvider& requiredVulkanExtensionsProvider):
        _appInfo(appInfo),
        _requiredVulkanExtensionsProvider(requiredVulkanExtensionsProvider),
        _instance(instantiateVulkan()) {}

        ~VulkanClient() = default;
    };

}
#endif //TUTORIAL_VULKAN_CLIENT_H