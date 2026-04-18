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

#include "Debug/Logger.h"
#include "Graphics/VulkanInstanceCreationStrategy.h"
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
        const Graphics::RequiredVulkanInstanceLayerProvider requiredVulkanInstanceLayerProvider;

        // VulkanInstanceの作成
        const Graphics::VulkanInstanceCreationStrategy vulkanInstanceCreationStrategy(extensionsProvider, requiredVulkanInstanceLayerProvider);
        const auto vulkanInstance = vulkanInstanceCreationStrategy.createVulkanInstance(appInfo);
        const auto vulkanInstanceReadModel = Graphics::VulkanInstanceReadModel(vulkanInstance.getHandler());

        // ウィンドウのサーフェスを作成する
        const WindowHelper::GlfwWindowSurfaceResourceAcquisition surfaceResourceAcquisition(applicationWindow.getHandler());
        const Graphics::VulkanSurface vulkanSurface(surfaceResourceAcquisition, vulkanInstance.getHandler());

        // 物理デバイスを選択
        const Graphics::VulkanPhysicalDeviceAPIVersionRequirements apiVersionRequirements;
        const Graphics::VulkanPhysicalDeviceQueueFamilyRequirements queueFamilyRequirements(vulkanSurface);
        const Graphics::VulkanPhysicalDeviceFeatureRequirements deviceFeatureRequirements;
        const Graphics::VulkanPhysicalDeviceExtensionsRequirements deviceExtensionRequirements;
        const Graphics::VulkanPhysicalDeviceSelectionStrategy physicalDeviceSelectionStrategy(
            apiVersionRequirements,
            queueFamilyRequirements,
            deviceFeatureRequirements,
            deviceExtensionRequirements
            );
        const auto vulkanPhysicalDevice = physicalDeviceSelectionStrategy.selectPhysicalDevice(vulkanInstanceReadModel);

        // 論理デバイスの作成
        const Graphics::VulkanLogicalDeviceCreationStrategy logicalDeviceCreationStrategy(
            queueFamilyRequirements,
            deviceFeatureRequirements,
            deviceExtensionRequirements
        );
        const auto vulkanLogicalDevice = logicalDeviceCreationStrategy.createLogicalDevice(vulkanPhysicalDevice);

        // 論理デバイスからキューファミリのインデックスを取得する
        const auto queueFamilyIndices = vulkanLogicalDevice.getQueueFamilyIndices();

        // キューファミリーのインデックスを指定してキューを取得する
        const auto vulkanDeviceQueue = vulkanLogicalDevice.getQueue(queueFamilyIndices[0], 0);

        // スワップチェーンの作成
        const Graphics::VulkanSwapChainCreateStrategy swapChainCreateStrategy(applicationWindow.getBufferSize());
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
