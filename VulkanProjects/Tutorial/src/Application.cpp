//
// Created by 沖田大河 on 2026/01/31.
//

#include "WindowHelper/ApplicationWindow.h"
#include <iostream>

#include "PlacementStackAllocator.h"
#include "SpanAllocator.h"
#include "Graphics/VulkanInstance.h"
#include "WindowHelper/WindowRequiredVulkanExtensionsProvider.h"
#include "Graphics/VulkanPhysicalDeviceSelectionStrategy.h"
#include "Application.h"

#include "Graphics/VulkanLogicalDeviceCreationStrategy.h"
#include "Graphics/VulkanSwapChainCreateStrategy.h"
#include "WindowHelper/GlfwWindowSurfaceResourceAcquisition.h"

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
        initializeVulkan(applicationWindow);

        // メインループを実行する
        mainLoop(applicationWindow);
        SpanAllocator::resetAllocator();
    }

    void Application::initializeVulkan(WindowHelper::ApplicationWindow& applicationWindow) {
        // ウィンドウが要求するVulkan拡張機能のプロバイダを作成してVulkanInstanceを作成する
        const WindowHelper::WindowRequiredVulkanExtensionsProvider extensionsProvider;
        const Graphics::VulkanInstance vulkanInstance(appInfo, extensionsProvider);
        const WindowHelper::GlfwWindowSurfaceResourceAcquisition surfaceResourceAcquisition(applicationWindow.getHandler());
        const Graphics::VulkanSurface vulkanSurface(surfaceResourceAcquisition, vulkanInstance.getHandler());
        const Graphics::VulkanPhysicalDeviceAPIVersionRequirements apiVersionRequirements;
        const Graphics::VulkanPhysicalDeviceQueueFamilyRequirements queueFamilyRequirements(vulkanSurface);
        const Graphics::VulkanPhysicalDeviceFeatureRequirements deviceFeatureRequirements;
        const Graphics::VulkanPhysicalDeviceExtensionsRequirements deviceExtensionRequirements;
        const Graphics::VulkanPhysicalDeviceSelectionStrategy physicalDeviceSelectionStrategy(
            vulkanInstance,
            apiVersionRequirements,
            queueFamilyRequirements,
            deviceFeatureRequirements,
            deviceExtensionRequirements
            );
        const Graphics::VulkanLogicalDeviceCreationStrategy logicalDeviceCreationStrategy(
            queueFamilyRequirements,
            deviceFeatureRequirements,
            deviceExtensionRequirements
        );
        const Graphics::VulkanSwapChainCreateStrategy swapChainCreateStrategy(applicationWindow.getBufferSize());

        const auto vulkanPhysicalDevice = physicalDeviceSelectionStrategy.selectPhysicalDevice();
        const auto vulkanLogicalDevice = logicalDeviceCreationStrategy.createLogicalDevice(vulkanPhysicalDevice);
        const auto queueFamilyIndices = vulkanLogicalDevice.getQueueFamilyIndices();
        const auto vulkanDeviceQueue = vulkanLogicalDevice.getQueue(queueFamilyIndices[0], 0);
        const auto vulkanSwapChain = swapChainCreateStrategy.createSwapChain(vulkanPhysicalDevice, vulkanSurface, vulkanLogicalDevice);
        const auto swapChainImages = vulkanSwapChain.getImages();

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
