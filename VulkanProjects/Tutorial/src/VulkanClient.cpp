//
// Created by 沖田大河 on 2026/02/14.
//

#include "VulkanClient.h"
#include "Logger.h"

#include <iostream>

#include "Span.h"

namespace Tutorial::Graphics {

#ifndef NDEBUG
    static constexpr const char* kValidationLayerName = "VK_LAYER_KHRONOS_validation";
#endif

    VkInstance VulkanClient::instantiateVulkan() const {
        const auto requiredExtensionCount = _requiredVulkanExtensionsProvider.getRequiredInstanceExtensionCount();
        const auto requiredExtensions = Span<char const*>::stackAlloc(requiredExtensionCount);
        _requiredVulkanExtensionsProvider.getRequiredInstanceExtensionNames(requiredExtensions);

        validateRequiredExtensions(requiredExtensions);

#ifndef NDEBUG
        if (!checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layer VK_LAYER_KHRONOS_validation is not supported");
        }
        constexpr const char* validationLayerName = kValidationLayerName;
        const VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debugCallback,
        };
        const VkInstanceCreateInfo instanceCreateInfo {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = &debugMessengerCreateInfo,
            .flags = _requiredVulkanExtensionsProvider.getRequiredVulkanInstanceCreateFlagBits(),
            .pApplicationInfo = &_appInfo,
            .enabledLayerCount = 1,
            .ppEnabledLayerNames = &validationLayerName,
            .enabledExtensionCount = requiredExtensionCount,
            .ppEnabledExtensionNames = requiredExtensions.headPtr,
        };
#else
        const VkInstanceCreateInfo instanceCreateInfo {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .flags = _requiredVulkanExtensionsProvider.getRequiredVulkanInstanceCreateFlagBits(),
            .pApplicationInfo = &_appInfo,
            .enabledExtensionCount = requiredExtensionCount,
            .ppEnabledExtensionNames = requiredExtensions.headPtr,
        };
#endif
        VkInstance instance;
        if (const auto result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance: " + std::to_string(result));
        }
        return instance;
    }

    void VulkanClient::validateRequiredExtensions(const Span<char const*>& requiredExtensions) const {
        // 実際にサポートされている拡張機能一覧を取得して、必須の拡張機能がサポートされているかを確認する
        uint32_t supportedExtensionCount = getSupportedExtensionCount();
        const auto supportedExtensions = Span<VkExtensionProperties>::stackAlloc(supportedExtensionCount);
        if (const auto result = vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensions.headPtr); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance extensions: " + std::to_string(result));
        }
        // 必須の拡張機能がサポートされている拡張機能の中に存在しないなら例外をスローする
        for (uint32_t i = 0; i < requiredExtensions.maxElementCount; ++i) {
            const auto requiredExtension = requiredExtensions[i];
            if (!isExtensionSupported(requiredExtension, supportedExtensions)) {
                throw std::runtime_error("Required Vulkan extension not supported: " + std::string(requiredExtension));
            }
        }
    }

    uint32_t VulkanClient::getSupportedExtensionCount() const {
        uint32_t extensionCount = 0;
        if (const auto result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate Vulkan instance extensions: " + std::to_string(result));
        }
        return extensionCount;
    }

    bool VulkanClient::isExtensionSupported(const char* extensionName, const Span<VkExtensionProperties>& actualSupportedExtensions) const {
        for (uint32_t i = 0; i < actualSupportedExtensions.maxElementCount; ++i) {
            if (const auto actualSupportedExtension = actualSupportedExtensions[i]; strcmp(actualSupportedExtension.extensionName, extensionName) == 0) {
                Debug::Logger::log("extension:" + std::string(extensionName) + " is supported");
                return true;
            }
        }
        return false;
    }

    VulkanClient::~VulkanClient() {
#ifndef NDEBUG
        const auto destroyFunc = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT"));
        if (destroyFunc != nullptr) {
            destroyFunc(_instance, _debugMessenger, nullptr);
        }
#endif
        vkDestroyInstance(_instance, nullptr);
    }

#ifndef NDEBUG
    bool VulkanClient::checkValidationLayerSupport() const {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        const auto availableLayers = Span<VkLayerProperties>::stackAlloc(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.headPtr);
        for (uint32_t i = 0; i < layerCount; ++i) {
            if (strcmp(availableLayers[i].layerName, kValidationLayerName) == 0) {
                return true;
            }
        }
        return false;
    }

    void VulkanClient::setupDebugMessenger() {
        const VkDebugUtilsMessengerCreateInfoEXT createInfo {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debugCallback,
        };
        const auto createFunc = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT"));
        if (createFunc == nullptr) {
            throw std::runtime_error("Failed to get vkCreateDebugUtilsMessengerEXT");
        }
        if (const auto result = createFunc(_instance, &createInfo, nullptr, &_debugMessenger); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create debug messenger: " + std::to_string(result));
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanClient::debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        (void)messageSeverity;
        (void)messageType;
        (void)pUserData;
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }
#endif
}

