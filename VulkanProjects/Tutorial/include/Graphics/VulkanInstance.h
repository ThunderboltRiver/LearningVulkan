//
// Created by 沖田大河 on 2026/02/14.
//

#ifndef TUTORIAL_VULKAN_INSTANCE_H
#define TUTORIAL_VULKAN_INSTANCE_H

#include <vulkan/vulkan.h>

#include "VulkanInstanceReadModel.h"
#include "ResourceManagement.h"

namespace Tutorial::Graphics {
    /**
     * Vulkanのインスタンスを表すクラス
     */
    class VulkanInstance {
        OwnerShip<VkInstance> _vkInstance;

        [[nodiscard]] OwnerShip<VkInstance> resourceAcquisition(const VkInstanceCreateInfo& instanceCreateInfo) const;
    public:
        explicit VulkanInstance(const VkInstanceCreateInfo& instanceCreateInfo);

        [[nodiscard]] Borrowed<VkInstance> getHandler() const;

        // vkInstanceの所有権を持つのは一つのインスタンスのみにするためコピー禁止
        VulkanInstance(const VulkanInstance&) = delete;
        VulkanInstance& operator=(const VulkanInstance&) = delete;

        // ムーブコンストラクタ。所有権をムーブ元からムーブ先に移動する。
        VulkanInstance(VulkanInstance&& moveOrigin) noexcept;

        // ムーブ代入演算子。所有権をムーブ元からムーブ先に移動する。
        VulkanInstance& operator=(VulkanInstance&& moveOrigin) noexcept;

        ~VulkanInstance();
    };

}
#endif //TUTORIAL_VULKAN_INSTANCE_H
