//
// Created by 沖田大河 on 2026/04/18.
//

#ifndef TUTORIAL_VULKAN_INSTANCE_QUERY_H
#define TUTORIAL_VULKAN_INSTANCE_QUERY_H

#include <vulkan/vulkan.h>
#include "Span.h"
#include "VulkanPhysicalDevice.h"
#include "ResourceManagement/Borrowed.h"

namespace Tutorial::Graphics {
    namespace rsm = Tutorial::ResourceManagement;

    /**
     * VulkanInstanceのReadModelを表すクラス
     */
    class VulkanInstanceReadModel {
        rsm::Borrowed<VkInstance> _vkInstance;

    public:

        /**
         * コンストラクタ。VulkanInstanceから作成する。
         * @param vkInstance VulkanInstanceから借用したVkInstance
         */
        explicit VulkanInstanceReadModel(rsm::Borrowed<VkInstance> vkInstance);

        /**
         * VulkanInstanceから物理デバイスのハンドルの配列を取得する。
         * @return VulkanInstanceから物理デバイスのハンドルの配列
         */
        [[nodiscard]] Span<VulkanPhysicalDevice> enumeratePhysicalDevices() const;
    };
}


#endif //TUTORIAL_VULKAN_INSTANCE_QUERY_H
