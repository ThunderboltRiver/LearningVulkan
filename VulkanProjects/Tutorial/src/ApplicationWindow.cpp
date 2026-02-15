//
// Created by 沖田大河 on 2026/02/04.
//

#include "ApplicationWindow.h"
#include <GLFW/glfw3.h>

namespace Tutorial::WindowHelper {

    GLFWwindow *ApplicationWindow::initWindow(uint32_t windowWidth, uint32_t windowHeight, const char* windowTitle) {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        return glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
    }

    bool ApplicationWindow::shouldClose() const {
        return glfwWindowShouldClose(_windowPtr);
    }

    void ApplicationWindow::pollEvents() const {
        glfwPollEvents();
    }

    ApplicationWindow::~ApplicationWindow() {
        glfwDestroyWindow(_windowPtr);
        glfwTerminate();
    }
}
