//
// Created by 沖田大河 on 2026/04/18.
//

#ifndef TUTORIAL_VULKAN_INSTANCE_CREATION_STRATEGY_H
#define TUTORIAL_VULKAN_INSTANCE_CREATION_STRATEGY_H

#include "VulkanInstance.h"
#include "IRequiredVulkanExtensionsProvider.h"
#include "RequiredVulkanInstanceLayerProvider.h"

namespace Tutorial::Graphics {
    class VulkanInstanceCreationStrategy {

        const IRequiredVulkanExtensionsProvider& _requiredVulkanExtensionsProvider;
        const RequiredVulkanInstanceLayerProvider& _requiredVulkanInstanceLayerProvider;
        public:

        explicit VulkanInstanceCreationStrategy(
            const IRequiredVulkanExtensionsProvider& requiredVulkanExtensionsProvider,
            const RequiredVulkanInstanceLayerProvider& requiredVulkanInstanceLayerProvider);

        /**
        * VulkanInstanceを作成する
        * @return 作成されたVulkanInstance
        */
        VulkanInstance createVulkanInstance(const VkApplicationInfo& applicationInfo) const;

        void validateRequiredLayer(const Span<char const *> &requiredLayerNames) const;

        bool isLayerSupported(const char *layerName, const Span<VkLayerProperties> &actualSupportedLayers) const;

        void validateRequiredExtensions(const Span<char const *> &requiredExtensions) const;

        uint32_t getSupportedExtensionCount() const;

        bool isExtensionSupported(const char *extensionName,
                                  const Span<VkExtensionProperties> &actualSupportedExtensions) const;
    };
}


#endif //TUTORIAL_VULKAN_INSTANCE_CREATION_STRATEGY_H