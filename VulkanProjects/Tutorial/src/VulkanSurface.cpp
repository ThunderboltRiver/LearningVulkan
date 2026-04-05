//
// Created by 沖田大河 on 2026/04/01.
//

#include "VulkanSurface.h"

namespace Tutorial::Graphics {
    VkSurfaceKHR VulkanSurface::getSurface() const {
        return _surface;
    }

    VulkanSurface::VulkanSurface(VkSurfaceKHR pureSurface, VkInstance instance):
        _surface(pureSurface),
        _instance(instance) {
    }

    VulkanSurface::VulkanSurface(VulkanSurface &&moveOrigin) noexcept {
        _surface = moveOrigin._surface;
        _instance = moveOrigin._instance;
        moveOrigin._surface = VK_NULL_HANDLE;
        moveOrigin._instance = VK_NULL_HANDLE;
    }

    VulkanSurface &VulkanSurface::operator=(VulkanSurface &&moveOrigin) noexcept {
        if (this != &moveOrigin) {
            if (_surface != VK_NULL_HANDLE) {
                // 既に所有しているsurfaceがある場合は解放する
                vkDestroySurfaceKHR(_instance, _surface, /*allocator*/nullptr);
            }
            _surface = moveOrigin._surface;
            _instance = moveOrigin._instance;
            moveOrigin._surface = VK_NULL_HANDLE;
            moveOrigin._instance = VK_NULL_HANDLE;
        }
        return *this;
    }

    VulkanSurface::~VulkanSurface() {
        if (_surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(_instance, _surface, nullptr);
        }
    }
}
