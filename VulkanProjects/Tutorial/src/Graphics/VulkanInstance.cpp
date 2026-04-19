//
// Created by 沖田大河 on 2026/02/14.
//

#include "Graphics/VulkanInstance.h"
#include "Debug/Logger.h"
#include <iostream>
#include <string>

namespace Tutorial::Graphics {
    namespace rsm = Tutorial::ResourceManagement;

    rsm::OwnerShip<VkInstance> VulkanInstance::resourceAcquisition(const VkInstanceCreateInfo &instanceCreateInfo) const {
        VkInstance instance;
        if (const auto result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance: " + std::to_string(result));
        }
        return rsm::OwnerShip(instance);
    }

    VulkanInstance::VulkanInstance(const VkInstanceCreateInfo &instanceCreateInfo):
        _vkInstance(resourceAcquisition(instanceCreateInfo)) {
        Debug::Logger::log("Vulkan instance created successfully");
    }

    rsm::Borrowed<VkInstance> VulkanInstance::getHandler() const {
        return _vkInstance.borrow();
    }

    VulkanInstance::VulkanInstance(VulkanInstance &&moveOrigin) noexcept:
        _vkInstance(moveOrigin._vkInstance.move()) {
        moveOrigin._vkInstance = rsm::OwnerShip<VkInstance>::MOVED();
    }

    VulkanInstance &VulkanInstance::operator=(VulkanInstance &&moveOrigin) noexcept {
        if (this != &moveOrigin) {
            if (_vkInstance.isNotMoved()) {
                // 既に所有しているインスタンスがある場合は解放する
                vkDestroyInstance(_vkInstance.getRawHandle(), nullptr);
            }
            _vkInstance = moveOrigin._vkInstance.move();
            moveOrigin._vkInstance = rsm::OwnerShip<VkInstance>::MOVED();
        }
        return *this;
    }

    VulkanInstance::~VulkanInstance() {
        if (_vkInstance.isNotMoved()) {
            vkDestroyInstance(_vkInstance.getRawHandle(), nullptr);
        }
    }
}
