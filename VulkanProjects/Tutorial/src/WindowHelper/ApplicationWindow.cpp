//
// Created by 沖田大河 on 2026/02/04.
//

#include "WindowHelper/ApplicationWindow.h"

#include <stdexcept>
#include <string>

namespace Tutorial::WindowHelper {
    namespace RM = Tutorial::ResourceManagement;

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

    RM::Borrowed<GLFWwindow *> ApplicationWindow::getHandler() const {
        return RM::Borrowed(_windowPtr);
    }

    bool ApplicationWindow::shouldClose() const {
        return glfwWindowShouldClose(_windowPtr);
    }

    void ApplicationWindow::pollEvents() const {
        glfwPollEvents();
    }
    
    Graphics::FrameBufferSize ApplicationWindow::getBufferSize() const {
        int32_t width, height;
        glfwGetFramebufferSize(_windowPtr, &width, &height);
        return Graphics::FrameBufferSize(Graphics::Pixel(width), Graphics::Pixel(height));
    }

    ApplicationWindow::~ApplicationWindow() {
        glfwDestroyWindow(_windowPtr);
        glfwTerminate();
    }
}
