//
// Created by 沖田大河 on 2026/03/02.
//

#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include "Span.h"
#include "WindowRequiredVulkanExtensionsProvider.h"

namespace Tutorial::WindowHelper {
    char const * const * WindowRequiredVulkanExtensionsProvider::getGlfwInstanceExtensionNames(uint32_t *pCount) const {
        const auto glfwRequiredExtensions = glfwGetRequiredInstanceExtensions(pCount);
        if (glfwRequiredExtensions == nullptr) {
            throw std::runtime_error("Failed to get required Vulkan extensions from GLFW");
        }
        return glfwRequiredExtensions;
    }

    uint32_t WindowRequiredVulkanExtensionsProvider::getRequiredInstanceExtensionCount() const {
        uint32_t count = 0;
        (void)getGlfwInstanceExtensionNames(&count);
        return count + 1; // GLFWが要求する拡張機能の数に、VK_KHR_portability_enumerationの分を加える
    }

    void WindowRequiredVulkanExtensionsProvider::getRequiredInstanceExtensionNames(const Span<char const*>& result) const {
#ifndef NDEBUG
        if (result.getMaxElementCount() != getRequiredInstanceExtensionCount()) {
            throw std::runtime_error("WindowRequiredVulkanExtensionsProvider: result span must have a maxElementCount equal to getRequiredInstanceExtensionCount()");
        }
#endif
        uint32_t count = 0;
        const auto glfwRequiredExtensions = getGlfwInstanceExtensionNames(&count);
        for (uint32_t i = 0; i < count; ++i) {
            *(result.pointerAt(i)) = glfwRequiredExtensions[i];
        }
        *(result.pointerAt(count)) = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
    }

    VkInstanceCreateFlags WindowRequiredVulkanExtensionsProvider::getRequiredVulkanInstanceCreateFlagBits() const {
        constexpr auto flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        return flags;
    }
}
