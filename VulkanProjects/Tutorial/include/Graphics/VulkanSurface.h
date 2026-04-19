//
// Created by 沖田大河 on 2026/04/01.
//

#ifndef TUTORIAL_VULKAN_SURFACE_H
#define TUTORIAL_VULKAN_SURFACE_H

#include <vulkan/vulkan.h>
#include "IVkSurfaceKHRResourceAcquisition.h"
#include "ResourceManagement/OwnerShip.h"

namespace Tutorial::Graphics {
    namespace RM = Tutorial::ResourceManagement;

    /**
     * VulkanのVkSurfaceKHRを薄くラップするクラス
     */
    class VulkanSurface {
        RM::OwnerShip<VkSurfaceKHR> _surface;
        RM::Borrowed<VkInstance> _instance;

    public:
        /**
         * このVulkanSurfaceが所有するVkSurfaceKHRへの借用を返す
         * @return このVulkanSurfaceが所有するVkSurfaceKHRへの借用
         */
        [[nodiscard]] RM::Borrowed<VkSurfaceKHR> getHandler() const;

        /**
         * コンストラクタ。VkSurfaceKHRを受け取ってVulkanSurfaceを作成する。
         * @param resourceAcquisition VkSurfaceKHRのリソース獲得を実行するためのオブジェクト。VkSurfaceKHRの作成に使用される
         * @param instance surfaceの作成に使用されたVkInstance。解放の際に必要
         */
        explicit VulkanSurface(const IVkSurfaceKHRResourceAcquisition& resourceAcquisition, RM::Borrowed<VkInstance> instance);

        // コピー禁止。VulkanSurfaceの所有権を持つのは一つのインスタンスのみにするため
        VulkanSurface(const VulkanSurface&) = delete;
        VulkanSurface& operator=(const VulkanSurface&) = delete;

        /**
         * ムーブコンストラクタ。所有権をムーブ元からムーブ先に移動する。
         * @param moveOrigin
         */
        VulkanSurface(VulkanSurface&& moveOrigin) noexcept;

        /**
         * ムーブ代入演算子。所有権をムーブ元からムーブ先に移動する。
         * @param moveOrigin
         * @return
         */
        VulkanSurface& operator=(VulkanSurface&& moveOrigin) noexcept;

        /**
         * デストラクタ。所有権を持つVulkanSurfaceが破棄される際に、VkSurfaceKHRを解放する
         */
        ~VulkanSurface();
    };
}
#endif //TUTORIAL_VULKAN_SURFACE_H
