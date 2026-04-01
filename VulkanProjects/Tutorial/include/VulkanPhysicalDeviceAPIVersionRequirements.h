//
// Created by 沖田大河 on 2026/03/15.
//

#ifndef TUTORIAL_VULKAN_PHYSICAL_DEVICE_API_VERSION_REQUIREMENTS_H
#define TUTORIAL_VULKAN_PHYSICAL_DEVICE_API_VERSION_REQUIREMENTS_H
#include "VulkanPhysicalDevice.h"

namespace Tutorial::Graphics {

    /**
     * Vulkan APIバージョンに対する要求を表すクラス
     */
    class VulkanPhysicalDeviceAPIVersionRequirements {

    public:
        VulkanPhysicalDeviceAPIVersionRequirements() = default;

        /**
         * 物理デバイスがサポートするVulkan APIバージョンがこの要求を満たすか
         * @param physicalDevice 確認対象の物理デバイス
         * @return この要求を満たすならtrue、そうでないならfalse
         */
        bool isSatisfiedBy(const VulkanPhysicalDevice &physicalDevice) const;
    };
}

#endif //TUTORIAL_VULKAN_PHYSICAL_DEVICE_API_VERSION_REQUIREMENTS_H