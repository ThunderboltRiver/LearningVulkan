//
// Created by 沖田大河 on 2026/03/20.
//

#include "../include/VulkanDeviceQueue.h"

namespace Tutorial::Graphics {
    VulkanDeviceQueue::VulkanDeviceQueue(VkQueue queue) : _queue(queue) {}
} // Graphics