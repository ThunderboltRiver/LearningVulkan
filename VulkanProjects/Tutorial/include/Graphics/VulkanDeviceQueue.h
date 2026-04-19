//
// Created by 沖田大河 on 2026/03/20.
//

#ifndef TUTORIAL_VULKAN_DEVICE_QUEUE_H
#define TUTORIAL_VULKAN_DEVICE_QUEUE_H

#include "vulkan/vulkan.h"
#include "ResourceManagement/Borrowed.h"
#include "ResourceManagement/Alias.h"

namespace Tutorial::Graphics {
    /**
     * Vulkanのキューを表すクラス
     */
    class VulkanDeviceQueue {

        /**
         * Vulkanのキューのハンドル
         * Vulkanのキューは、論理デバイスが実質的に所有権をもっているため、借用で保持している
         */
        rsm::Borrowed<VkQueue> _queue;

        /**
         * このキューのインデックス
         */
        uint32_t queueIndex;

        /**
         * このキューが属するキューファミリのインデックス
         */
        uint32_t queueFamilyIndex;

    public:
        explicit VulkanDeviceQueue(rsm::Borrowed<VkQueue> queue, uint32_t queueIndex, uint32_t queueFamilyIndex);

        [[nodiscard]] rsm::Borrowed<VkQueue> getHandle() const;

         [[nodiscard]] uint32_t getQueueFamilyIndex() const;

         [[nodiscard]] uint32_t getQueueIndex() const;
    };
}

#endif //TUTORIAL_VULKAN_DEVICE_QUEUE_H
