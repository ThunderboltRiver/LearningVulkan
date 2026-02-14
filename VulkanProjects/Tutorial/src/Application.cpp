//
// Created by 沖田大河 on 2026/01/31.
//

#include "ApplicationWindow.h"
#include <iostream>

#include "VulkanClient.h"
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#   include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif
#include "Application.h"
using namespace Tutorial::WindowHelper;

namespace Tutorial
{
    Application::Application() {}

    void Application::run() {
        // 自身のウィンドウを作成
        ApplicationWindow applicationWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
        constexpr vk::ApplicationInfo appInfo {
            .pApplicationName = "Tutorial",
            .applicationVersion =  VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "No Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = vk::ApiVersion14
        };
        Graphics::VulkanClient vkClient(appInfo, _extensionsProvider);
        mainLoop(applicationWindow);
    }

    void Application::mainLoop(const ApplicationWindow& applicationWindow) {
        while (!applicationWindow.shouldClose()) {
            applicationWindow.pollEvents();
        }
    }

    void Application::throwableResourceCleanup()
    {
        std::cout << "throwable resource cleanup" << std::endl;
    }

    Application::~Application()
    {
        std::cout << "destructor" << std::endl;
    }
};
