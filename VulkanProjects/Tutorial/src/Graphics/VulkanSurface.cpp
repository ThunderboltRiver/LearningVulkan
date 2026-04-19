//
// Created by 沖田大河 on 2026/04/01.
//

#include "Graphics/VulkanSurface.h"

namespace Tutorial::Graphics {
    namespace rsm = Tutorial::ResourceManagement;

    rsm::Borrowed<VkSurfaceKHR> VulkanSurface::getHandler() const { return _surface.borrow(); }

    VulkanSurface::VulkanSurface(const IVkSurfaceKHRResourceAcquisition &resourceAcquisition, rsm::Borrowed<VkInstance> instance):
        _surface(resourceAcquisition.execute(instance)),
        _instance(instance) {
    }

    VulkanSurface::VulkanSurface(VulkanSurface &&moveOrigin) noexcept:
        _surface(moveOrigin._surface.move()),
        _instance(moveOrigin._instance) {
        moveOrigin._surface = rsm::OwnerShip<VkSurfaceKHR>::MOVED();
        moveOrigin._instance = rsm::Borrowed<VkInstance>::Null();
    }

    VulkanSurface &VulkanSurface::operator=(VulkanSurface &&moveOrigin) noexcept {
        if (this != &moveOrigin) {
            if (_surface.isNotMoved()) {
                // 既に所有しているsurfaceがある場合は解放する
                vkDestroySurfaceKHR(_instance.getRawHandle(), _surface.getRawHandle(), /*allocator*/nullptr);
            }
            _surface = moveOrigin._surface.move();
            _instance = moveOrigin._instance;
            moveOrigin._surface = rsm::OwnerShip<VkSurfaceKHR>::MOVED();
            moveOrigin._instance = rsm::Borrowed<VkInstance>::Null();
        }
        return *this;
    }

    VulkanSurface::~VulkanSurface() {
        if (_surface.isNotMoved()) {
            vkDestroySurfaceKHR(_instance.getRawHandle(), _surface.getRawHandle(), nullptr);
        }
    }
}
