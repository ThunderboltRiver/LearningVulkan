//
// Created by 沖田大河 on 2026/03/15.
//

#ifndef TUTORIAL_VULKANPHYSICALDEVICEEXTENSIONSREQUIREMENTS_H
#define TUTORIAL_VULKANPHYSICALDEVICEEXTENSIONSREQUIREMENTS_H
#include "Span.h"
#include "VulkanPhysicalDevice.h"

namespace Tutorial::Graphics {
    /**
     * 物理デバイス拡張機能に対する要求を表すクラス
     */
    class VulkanPhysicalDeviceExtensionsRequirements {

        /**
         * 物理デバイスが必要なデバイス拡張機能をサポートしているか
         * @param supportedDeviceExtensions 物理デバイスがサポートするデバイス拡張機能の配列
         * @param requiredExtensionName サポートが必要なデバイス拡張機能の名前
         * @return サポートが必要なデバイス拡張機能をサポートしているならtrue、そうでないならfalse
         */
        bool isRequiredDeviceExtensionSupported(const Span<VkExtensionProperties>& supportedDeviceExtensions, const char* requiredExtensionName) const;

    public:
        VulkanPhysicalDeviceExtensionsRequirements() = default;

        /**
         * 物理デバイスの拡張機能がこの要求を満たすか
         * @param physicalDevice 確認対象の物理デバイス
         * @return この要求を満たすならtrue、そうでないならfalse
         */
        bool isSatisfiedBy(const VulkanPhysicalDevice &physicalDevice) const;

        /**
         * この要件をDevice拡張名称の配列として返す
         * @return この要件をDevice拡張名称の配列として返す
         */
        [[nodiscard]] Span<const char*> AsVkDeviceExtensionNames() const;
    };
}

#endif //TUTORIAL_VULKANPHYSICALDEVICEEXTENSIONSREQUIREMENTS_H