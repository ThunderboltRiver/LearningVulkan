//
// Created by 沖田大河 on 2026/04/01.
//

#ifndef TUTORIAL_VULKAN_SURFACE_H
#define TUTORIAL_VULKAN_SURFACE_H
#include <vulkan/vulkan_core.h>

namespace Tutorial::Graphics {

    /**
     * VulkanのVkSurfaceKHRを薄くラップするクラス
     */
    class VulkanSurface {
        VkSurfaceKHR _surface;
        VkInstance _instance;

    public:

        [[nodiscard]] VkSurfaceKHR getSurface() const;

        /**
         * コンストラクタ。VkSurfaceKHRを受け取ってVulkanSurfaceを作成する。
         * @param pureSurface VulkanSurfaceが所有するVkSurfaceKHR
         * @param instance surfaceの作成に使用されたVkInstance。解放の際に必要
         */
        explicit VulkanSurface(VkSurfaceKHR pureSurface, VkInstance instance);

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