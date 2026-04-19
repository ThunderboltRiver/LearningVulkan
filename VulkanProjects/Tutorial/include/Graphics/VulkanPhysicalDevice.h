//
// Created by 沖田大河 on 2026/03/14.
//

#ifndef TUTORIAL_VULKAN_PHYSICAL_DEVICE_H
#define TUTORIAL_VULKAN_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>
#include "Span.h"
#include "ResourceManagement/OwnerShip.h"
#include "ResourceManagement/Alias.h"

namespace Tutorial::Graphics {
    /**
     * Vulkanの物理デバイスを表すクラス
     */
    class VulkanPhysicalDevice {

        // 物理デバイスのハンドル
        // 物理デバイスに関しては、VulkanInstanceから取得したハンドルを使用するだけで、特にリソースの解放なども必要ないため、借用で保持する
        rsm::Borrowed<VkPhysicalDevice> _physicalDevice;

    public:
        explicit VulkanPhysicalDevice(rsm::Borrowed<VkPhysicalDevice> physicalDevice) : _physicalDevice(physicalDevice) {}

        [[nodiscard]] rsm::Borrowed<VkPhysicalDevice> getHandle() const;

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
         * vkGetPhysicalDeviceSurfaceCapabilitiesKHR のラッパー
         * VK_KHR_surface 拡張機能が有効化されている物理デバイスでのみ使用可能
         */
        VkResult getSurfaceCapabilitiesKHR(rsm::Borrowed<VkSurfaceKHR> surface, VkSurfaceCapabilitiesKHR *pSurfaceCapabilities) const;

        /**
         * vkGetPhysicalDeviceSurfaceCapabilities2KHR のラッパー
         * VK_KHR_get_surface_capabilities2　拡張機能が有効化されている物理デバイスでのみ使用可能
         */
        VkResult getSurfaceCapabilities2KHR(const VkPhysicalDeviceSurfaceInfo2KHR *pSurfaceInfo, VkSurfaceCapabilities2KHR *pSurfaceCapabilities) const;

        /**
         * vkGetPhysicalDeviceSurfaceFormatsKHR のラッパー
         * VK_KHR_surface 拡張機能が有効化されている物理デバイスでのみ使用可能
         */
        VkResult getSurfaceFormatsKHR(rsm::Borrowed<VkSurfaceKHR> surface, uint32_t *pSurfaceFormatCount, VkSurfaceFormatKHR *pSurfaceFormat) const;

        /**
         * vkGetPhysicalDeviceSurfaceFormats2KHR のラッパー
         * VK_KHR_get_surface_capabilities2　拡張機能が有効化されている物理デバイスでのみ使用可能
         */
        VkResult getPhysicalDeviceSurfaceFormats2KHR(const VkPhysicalDeviceSurfaceInfo2KHR *pSurfaceInfo, uint32_t *pSurfaceFormatCount, VkSurfaceFormat2KHR *pSurfaceFormats) const;

        /**
         * vkGetPhysicalDeviceSurfacePresentModesKHR のラッパー
         */
        VkResult getPhysicalDeviceSurfacePresentModeKHR(rsm::Borrowed<VkSurfaceKHR> surface, uint32_t *pPresentModeCount, VkPresentModeKHR *pPresentModes) const;

#ifdef VK_USE_PLATFORM_WIN32_KHR
        /**
         * vkGetPhysicalDeviceSurfacePresentModes2EXT のラッパー
         * この関数は、VK_USE_PLATFORM_WIN32_KHRが定義されている場合にのみ使用可能
         */
        VkResult getPhysicalDeviceSurfacePresentModes2Ext(const VkPhysicalDeviceSurfaceInfo2KHR *pSurfaceInfo, uint32_t *pPresentModeCount, VkPresentModeKHR *pPresentModes) const;
#endif

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
        bool isPresentationSupported(uint32_t queueFamilyIndex, rsm::Borrowed<VkSurfaceKHR> surface) const;
    };
} // Graphics

#endif //TUTORIAL_VULKAN_PHYSICAL_DEVICE_H
