//
// Created by 沖田大河 on 2026/03/20.
//

#include "Graphics/VulkanDeviceQueue.h"
#include "ResourceManagement.h"

namespace Tutorial::Graphics {
    VulkanDeviceQueue::VulkanDeviceQueue(Borrowed<VkQueue> queue, uint32_t queueIndex, uint32_t queueFamilyIndex):
        _queue(queue),
        queueIndex(queueIndex),
        queueFamilyIndex(queueFamilyIndex) {
    }
    
    Borrowed<VkQueue> VulkanDeviceQueue::getHandle() const {
        return _queue;
    }

    uint32_t VulkanDeviceQueue::getQueueIndex() const {
        return queueIndex;
    }

    uint32_t VulkanDeviceQueue::getQueueFamilyIndex() const {
        return queueFamilyIndex;
    }
} // Graphics
