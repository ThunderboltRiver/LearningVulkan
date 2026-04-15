//
// Created by 沖田大河 on 2026/02/14.
//

#ifndef TUTORIAL_VULKAN_INSTANCE_H
#define TUTORIAL_VULKAN_INSTANCE_H

#include <sys/types.h>

#include "Graphics/IRequiredVulkanExtensionsProvider.h"
#include "Span.h"
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
# include <vulkan/vulkan.h>
#else
import vulkan_hpp;
#endif

namespace Tutorial::Graphics {
    class VulkanInstance {
        const VkApplicationInfo& _appInfo;
        const IRequiredVulkanExtensionsProvider& _requiredVulkanExtensionsProvider;
        constexpr static char const* validationLayerNames[] = { "VK_LAYER_KHRONOS_validation" };
#ifndef NDEBUG
        constexpr static bool enableValidationLayers = true;
#else
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
        VulkanInstance(const VkApplicationInfo& appInfo, const IRequiredVulkanExtensionsProvider& requiredVulkanExtensionsProvider):
        _appInfo(appInfo),
        _requiredVulkanExtensionsProvider(requiredVulkanExtensionsProvider),
        _instance(instantiateVulkan()) {}

        const VkInstance& getInstance() const;

        // vkInstanceの所有権を持つのは一つのインスタンスのみにするためコピー禁止
        VulkanInstance(const VulkanInstance&) = delete;
        VulkanInstance& operator=(const VulkanInstance&) = delete;

        // 物理デバイスの数を返す
        [[nodiscard]] uint32_t getPhysicalDevicesCount() const;

        // 物理デバイスのハンドルをresultに格納する。resultはgetPhysicalDevicesCount()で返される数の要素を持つSpanでなければならない
        void enumeratePhysicalDevices(Span<VkPhysicalDevice> &result) const;

        ~VulkanInstance();
    };

}
#endif //TUTORIAL_VULKAN_INSTANCE_H