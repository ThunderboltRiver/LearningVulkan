//
// Created by 沖田大河 on 2026/01/31.
//

#include "ApplicationWindow.h"
#include <iostream>

#include "VulkanClient.h"
#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan.h>
#else
import vulkan_hpp;
#endif
#include "Application.h"

namespace Tutorial
{
    Application::Application() {}

    void Application::run() {
        // 自身のウィンドウを作成
        WindowHelper::ApplicationWindow applicationWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
        constexpr VkApplicationInfo appInfo {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "Tutorial",
            .applicationVersion =  VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "No Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_4
        };
        Graphics::VulkanClient vkClient(appInfo, _extensionsProvider);
        mainLoop(applicationWindow);
    }

    void Application::mainLoop(const WindowHelper::ApplicationWindow& applicationWindow) {
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
