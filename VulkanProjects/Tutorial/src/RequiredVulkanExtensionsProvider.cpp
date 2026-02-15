//
// Created by 沖田大河 on 2026/02/14.
//

#include <vector>
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan.h>
#else
import vulkan_hpp;
#endif
#include "RequiredVulkanExtensionsProvider.h"
#include <GLFW/glfw3.h>

namespace Tutorial::Graphics {

    std::vector<const char*> RequiredVulkanExtensionsProvider::getRequiredVulkanExtensionNames() const {
        uint32_t count = 0;
        auto glfwRequiredExtensions = glfwGetRequiredInstanceExtensions(&count);
        std::vector<const char*> requiredExtensionNames;
        requiredExtensionNames.reserve(count + 1);
        requiredExtensionNames.insert(requiredExtensionNames.end(),
                              glfwRequiredExtensions,
                              glfwRequiredExtensions + count);
        requiredExtensionNames.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        return requiredExtensionNames;
    }

    VkInstanceCreateFlagBits RequiredVulkanExtensionsProvider::getRequiredVulkanInstanceCreateFlagBits() const {
        return VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }
}
