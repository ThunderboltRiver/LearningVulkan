//
// Created by 沖田大河 on 2026/01/31.
//

#include <iostream>
#include <Application.h>

#include "ApplicationWindow.h"
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#   include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#include <GLFW/glfw3.h>

namespace Tutorial
{
    Application::Application() {}

    void Application::run() {
        // 自身のウィンドウを作成
        ApplicationWindow applicationWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
        mainLoop(applicationWindow);
    }

    void Application::mainLoop(const ApplicationWindow &applicationWindow) {
        while (!applicationWindow.shouldClose()) {
            applicationWindow.pollEvents();
        }
    }

    void Application::initializeGraphicsAPI()
    {
        std::cout << "Tutorial Application initializeGraphicsAPI" << std::endl;
    }

    void Application::throwableResourceCleanup()
    {
        std::cout << "throwable resource cleanup" << std::endl;
    }

    Application::~Application()
    {
        std::cout << "destructor" << std::endl;
    }
}
