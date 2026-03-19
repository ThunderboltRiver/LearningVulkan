//
// Created by 沖田大河 on 2026/03/15.
//

#include "VulkanPhysicalDeviceQueueFamilyRequirements.h"

namespace Tutorial::Graphics {

    bool VulkanPhysicalDeviceQueueFamilyRequirements::findSatisfiedQueueFamilyIndex(const VulkanPhysicalDevice &physicalDevice,
                                                                                    uint32_t *queueFamilyIndex) const {
        // 物理デバイスがサポートするキューファミリーのプロパティの配列を取得する
        const auto& queueFamilies = getQueueFamilyProperties(physicalDevice);

        // 物理デバイスが必要なキューファミリーのプロパティをサポートしているかを確認する
        uint32_t index = 0;
        for (auto& queueFamily : queueFamilies) {
            // グラフィックスコマンドをサポートしているキューファミリーが存在するならこの物理デバイスは必要なキューファミリーのプロパティをサポートしているとみなす
            if ((queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
                *queueFamilyIndex = index;
                return true;
            }
            ++index;
        }
        return false;
    }

    const Span<VkQueueFamilyProperties2>& VulkanPhysicalDeviceQueueFamilyRequirements::getQueueFamilyProperties(const VulkanPhysicalDevice &physicalDevice) const {
        // 物理デバイスがサポートするキューファミリーのプロパティの配列を取得する
        uint32_t queueFamilyPropertyCount = 0;
        physicalDevice.getQueueFamilyProperties2(&queueFamilyPropertyCount, nullptr);
        const auto& queueFamilies = Span<VkQueueFamilyProperties2>::stackAlloc(queueFamilyPropertyCount);
        for (auto& queueFamilyProperty : queueFamilies) {
            queueFamilyProperty.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
            queueFamilyProperty.pNext = nullptr;
        }
        physicalDevice.getQueueFamilyProperties2(&queueFamilyPropertyCount, queueFamilies.headPtr);
        return queueFamilies;
    }
} // Graphics