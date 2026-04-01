//
// Created by 沖田大河 on 2026/03/20.
//

#ifndef TUTORIAL_VULKAN_DEVICE_QUEUE_H
#define TUTORIAL_VULKAN_DEVICE_QUEUE_H
#include "vulkan/vulkan.h"

namespace Tutorial::Graphics {

    /**
     * Vulkanのキューを表すクラス
     */
    class VulkanDeviceQueue {
        VkQueue _queue;
    public:

        explicit VulkanDeviceQueue(VkQueue queue) : _queue(queue) {}
    };
}

#endif //TUTORIAL_VULKAN_DEVICE_QUEUE_H