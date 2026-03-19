//
// Created by 沖田大河 on 2026/03/15.
//

#ifndef TUTORIAL_VULKAN_PHYSICAL_DEVICE_QUEUE_FAMILY_REQUIREMENTS_H
#define TUTORIAL_VULKAN_PHYSICAL_DEVICE_QUEUE_FAMILY_REQUIREMENTS_H

#include "Span.h"
#include "VulkanPhysicalDevice.h"

namespace Tutorial::Graphics {

    /**
     * 物理デバイスのキューファミリーに対する要求を表すクラス
     */
    class VulkanPhysicalDeviceQueueFamilyRequirements {

        /**
         * 物理デバイスがサポートするキューファミリーのプロパティの配列を取得する
         * @param physicalDevice 物理デバイス
         * @return 物理デバイスがサポートするキューファミリーのプロパティの配列
         */
        [[nodiscard]] const Span<VkQueueFamilyProperties2>& getQueueFamilyProperties(const VulkanPhysicalDevice &physicalDevice) const;

    public:
        VulkanPhysicalDeviceQueueFamilyRequirements() = default;

        /**
         *　物理デバイスがサポートするキューファミリーの中からこの要求を満たすものを見つけ、そのインデックスを取得する
         * @param physicalDevice 確認対象の物理デバイス
         * @param queueFamilyIndex この要求を満たすキューファミリーのインデックスを格納するためのポインタ
         * @return この要求を満たすキューファミリーのインデックスを見つけることができたならtrue、そうでないならfalse
         */
        bool findSatisfiedQueueFamilyIndex(const VulkanPhysicalDevice &physicalDevice, uint32_t* queueFamilyIndex) const;
    };

};

#endif //TUTORIAL_VULKAN_PHYSICAL_DEVICE_QUEUE_FAMILY_REQUIREMENTS_H