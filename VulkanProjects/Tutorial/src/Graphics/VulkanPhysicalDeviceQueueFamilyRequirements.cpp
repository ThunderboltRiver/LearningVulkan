//
// Created by 沖田大河 on 2026/03/15.
//

#include "Graphics/VulkanPhysicalDeviceQueueFamilyRequirements.h"

#include "Debug/Logger.h"

namespace Tutorial::Graphics {

    bool VulkanPhysicalDeviceQueueFamilyRequirements::findSatisfiedQueueFamilyIndex(const VulkanPhysicalDevice &physicalDevice,
                                                                                    uint32_t *queueFamilyIndex) const {
        // 物理デバイスがサポートするキューファミリーのプロパティの配列を取得する
        const auto queueFamilies = getQueueFamilyProperties(physicalDevice);

        // 物理デバイスが必要なキューファミリーのプロパティをサポートしているかを確認する
        uint32_t index = 0;

        for (auto& queueFamily: queueFamilies) {
            // グラフィックスコマンドとサーフェスへのプレゼンテーションをサポートしているキューファミリーが存在するなら
            // この物理デバイスは必要なキューファミリーのプロパティをサポートしているとみなす
            const auto isGraphicsSupported = (queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0;
            const auto isPresentationSupported = physicalDevice.isPresentationSupported(index, _vulkanSurface.getHandler());
            if (isGraphicsSupported && isPresentationSupported) {
                *queueFamilyIndex = index;
                return true;
            }
            ++index;
        }
        return false;
    }

    Span<VkQueueFamilyProperties2> VulkanPhysicalDeviceQueueFamilyRequirements::getQueueFamilyProperties(const VulkanPhysicalDevice &physicalDevice) const {
        // 物理デバイスがサポートするキューファミリーのプロパティの配列を取得する
        uint32_t queueFamilyPropertyCount = 0;
        physicalDevice.getQueueFamilyProperties2(&queueFamilyPropertyCount, VK_NULL_HANDLE);
        auto queueFamilies = Span<VkQueueFamilyProperties2>::stackAlloc(queueFamilyPropertyCount);
        for (uint32_t i = 0; i < queueFamilyPropertyCount; ++i) {
            auto* prop = queueFamilies.mutablePointerAt(i);
            prop->sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
            prop->pNext = VK_NULL_HANDLE;
        }
        Debug::Logger::log("Physical device supports " + std::to_string(queueFamilyPropertyCount) + " queue families pointer: " + std::to_string(reinterpret_cast<uintptr_t>(queueFamilies.getHeadPtr())));
        physicalDevice.getQueueFamilyProperties2(&queueFamilyPropertyCount, queueFamilies.getHeadPtr());
        queueFamilies.markFilled();
        return queueFamilies;
    }

    VulkanPhysicalDeviceQueueFamilyRequirements::VulkanPhysicalDeviceQueueFamilyRequirements(
        const VulkanSurface &surface) : _vulkanSurface(surface) {
    }
} // Graphics