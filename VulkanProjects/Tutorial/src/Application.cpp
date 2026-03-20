//
// Created by 沖田大河 on 2026/01/31.
//

#include "ApplicationWindow.h"
#include <iostream>

#include "PlacementStackAllocator.h"
#include "SpanAllocator.h"
#include "VulkanInstance.h"
#include "WindowRequiredVulkanExtensionsProvider.h"
#include "VulkanPhysicalDeviceSelectionStrategy.h"
#include "Application.h"

#include "VulkanLogicalDeviceCreationStrategy.h"

namespace Tutorial
{
    Application::Application() = default;

    void Application::run() {

        // PlacementStackAllocatorのインスタンスを作成してSpanにセットする
        PlacementStackAllocator allocator(STACK_ALLOCATOR_12_MB_CAPACITY);
        SpanAllocator::setAllocator(&allocator);

        // 自身のウィンドウを作成
        WindowHelper::ApplicationWindow applicationWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

        // Vulkanを初期化する
        initializeVulkan();

        // メインループを実行する
        mainLoop(applicationWindow);
        SpanAllocator::resetAllocator();
    }

    void Application::initializeVulkan() {
        // ウィンドウが要求するVulkan拡張機能のプロバイダを作成してVulkanClientを作成する
        const WindowHelper::WindowRequiredVulkanExtensionsProvider extensionsProvider;

        const Graphics::VulkanInstance vulkanInstance(appInfo, extensionsProvider);
        const Graphics::VulkanPhysicalDeviceSelectionStrategy physicalDeviceSelectionStrategy(vulkanInstance);
        const Graphics::VulkanLogicalDeviceCreationStrategy logicalDeviceCreationStrategy;
        const auto vulkanPhysicalDevice = physicalDeviceSelectionStrategy.selectPhysicalDevice();
        const auto vulkanLogicalDevice = logicalDeviceCreationStrategy.createLogicalDevice(vulkanPhysicalDevice);


    }

    Application::~Application()
    {
        std::cout << "destructor" << std::endl;
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
};
