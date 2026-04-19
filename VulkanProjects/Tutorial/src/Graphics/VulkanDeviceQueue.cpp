//
// Created by 沖田大河 on 2026/03/20.
//

#include "Graphics/VulkanDeviceQueue.h"

namespace Tutorial::Graphics {
    namespace rsm = Tutorial::ResourceManagement;

    VulkanDeviceQueue::VulkanDeviceQueue(rsm::Borrowed<VkQueue> queue, uint32_t queueIndex, uint32_t queueFamilyIndex):
        _queue(queue),
        queueIndex(queueIndex),
        queueFamilyIndex(queueFamilyIndex) {
    }
    
    rsm::Borrowed<VkQueue> VulkanDeviceQueue::getHandle() const {
        return _queue;
    }

    uint32_t VulkanDeviceQueue::getQueueIndex() const {
        return queueIndex;
    }

    uint32_t VulkanDeviceQueue::getQueueFamilyIndex() const {
        return queueFamilyIndex;
    }
} // Graphics
