//
// Created by 沖田大河 on 2026/04/17.
//

#include "WindowHelper/GlfwWindowSurfaceResourceAcquisition.h"
#include <stdexcept>
#include <string>
#include "ResourceManagement/Alias.h"

namespace Tutorial::WindowHelper {
    GlfwWindowSurfaceResourceAcquisition::GlfwWindowSurfaceResourceAcquisition(rsm::Borrowed<GLFWwindow*> windowHandler):
        _windowHandler(windowHandler) {
    }

    rsm::OwnerShip<VkSurfaceKHR> GlfwWindowSurfaceResourceAcquisition::execute(rsm::Borrowed<VkInstance> instance) const {
        VkSurfaceKHR surface;
        if (const auto result = glfwCreateWindowSurface(instance.getRawHandle(), _windowHandler.getRawHandle(), nullptr, &surface); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan surface: " + std::to_string(result));
        }
        return rsm::OwnerShip(surface);
    }
}
