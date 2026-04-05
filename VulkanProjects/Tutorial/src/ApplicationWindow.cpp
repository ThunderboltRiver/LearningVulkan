//
// Created by 沖田大河 on 2026/02/04.
//

#include "ApplicationWindow.h"

#include <stdexcept>
#include <string>

namespace Tutorial::WindowHelper {

    GLFWwindow *ApplicationWindow::initWindow(uint32_t windowWidth, uint32_t windowHeight, const char* windowTitle) {
        if (auto resultOfInitGLFW = glfwInit(); resultOfInitGLFW != GLFW_TRUE) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        const auto windowHandler = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
        if (windowHandler == nullptr) {
            throw std::runtime_error("Failed to create GLFW window");
        }
        return windowHandler;
    }

    bool ApplicationWindow::shouldClose() const {
        return glfwWindowShouldClose(_windowPtr);
    }

    void ApplicationWindow::pollEvents() const {
        glfwPollEvents();
    }

    Graphics::VulkanSurface ApplicationWindow::createVulkanSurface(
        const Graphics::VulkanInstance &vulkanInstance) const {
        VkSurfaceKHR surface;
        if (const auto result = glfwCreateWindowSurface(vulkanInstance.getInstance(), _windowPtr, nullptr, &surface); result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan surface: " + std::to_string(result));
        }
        return Graphics::VulkanSurface(surface, vulkanInstance.getInstance());
    }

    ApplicationWindow::~ApplicationWindow() {
        glfwDestroyWindow(_windowPtr);
        glfwTerminate();
    }
}
