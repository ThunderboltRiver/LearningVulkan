//
// Created by 沖田大河 on 2026/03/14.
//

#ifndef TUTORIAL_VULKAN_PHYSICAL_DEVICE_H
#define TUTORIAL_VULKAN_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>

#include "Span.h"

namespace Tutorial::Graphics {

    /**
     * Vulkanの物理デバイスを表すクラス
     */
    class VulkanPhysicalDevice {

        // 物理デバイスのハンドル
        VkPhysicalDevice _physicalDevice;

    public:
        explicit VulkanPhysicalDevice(VkPhysicalDevice physicalDevice) : _physicalDevice(physicalDevice) {}

        void getProperties2(VkPhysicalDeviceProperties2& properties) const;

        void getQueueFamilyProperties2(uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) const;

        VkResult enumerateExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) const;

        void getFeatures2(VkPhysicalDeviceFeatures2& features) const;

        VkResult createDevice(const VkDeviceCreateInfo &deviceCreateInfo, const VkAllocationCallbacks *pAllocator,
                              VkDevice *pLogicalDevice) const;

        /**
         * vkGetPhysicalDeviceSurfaceSupportKHR のラッパー。
         */
        VkResult getSurfaceSupportKHR(uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) const;

        /**
         * この物理デバイスがサポートする拡張機能のプロパティからなる配列を取得する
         * @param pLayerName 拡張機能のプロパティを取得する対象のレイヤーの名前。nullptrを指定すると、レイヤーに依存しない拡張機能のプロパティが取得される
         * @return 物理デバイスがサポートする拡張機能のプロパティの配列
         */
        [[nodiscard]] Span<VkExtensionProperties> enumerateExtensionProperties(const char* pLayerName) const;

        /**
         * 指定されたキューファミリーが、指定されたサーフェスに対してプレゼンテーションをサポートしているかを返す。
         * @param queueFamilyIndex キューファミリーのインデックス
         * @param surface サーフェス
         * @return 指定されたキューファミリーが、指定されたサーフェスに対してプレゼンテーションをサポートしているならtrue、そうでないならfalse
         */
        bool isPresentationSupported(uint32_t queueFamilyIndex, VkSurfaceKHR surface) const;
    };
} // Graphics

#endif //TUTORIAL_VULKAN_PHYSICAL_DEVICE_H