function(find_packages_project_depends)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../Shared/CMake")
    find_package (Vulkan REQUIRED)
    find_package (glfw3 REQUIRED)
endfunction(find_packages_project_depends)

function(bind_vulkan_librally_to TARGET)
    if (ENABLE_CPP20_MODULE)
        set(CMAKE_CXX_SCAN_FOR_MODULES ON)
    endif()
    if (ENABLE_CPP20_MODULE)
        add_library(VulkanCppModule)
        add_library(Vulkan::cppm ALIAS VulkanCppModule)

        target_compile_definitions(VulkanCppModule PUBLIC
                VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
                VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
        )

        target_include_directories(VulkanCppModule PRIVATE "${Vulkan_INCLUDE_DIR}")

        target_link_libraries(VulkanCppModule PUBLIC Vulkan::Vulkan)

        set_target_properties(VulkanCppModule PROPERTIES CXX_STANDARD 20)

        target_sources(VulkanCppModule
                PUBLIC
                FILE_SET cxx_modules TYPE CXX_MODULES
                BASE_DIRS "${Vulkan_INCLUDE_DIR}"
                FILES "${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm"
        )
        target_link_libraries (${TARGET} Vulkan::cppm)
    else()
        target_compile_definitions(${TARGET} PUBLIC
                VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
                VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
        )

        target_include_directories(${TARGET} PRIVATE "${Vulkan_INCLUDE_DIR}")
        target_link_libraries(${TARGET} Vulkan::Vulkan)
    endif()
endfunction(bind_vulkan_librally_to)

function(bind_window_lib_to TARGET)
    target_link_libraries(${TARGET} glfw)
endfunction(bind_window_lib_to)

# Tutorialプロジェクトの作成
cmake_minimum_required (VERSION 3.29)
project(Tutorial LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tutorialターゲットの作成
add_executable(Tutorial
        src/main.cpp
        src/Application.cpp
        src/PlacementStackAllocator.cpp
        src/Span.cpp
        src/SpanAllocator.cpp
        src/Graphics/FrameBufferSize.cpp
        src/Graphics/Pixel.cpp
        src/Graphics/VulkanInstanceCreationStrategy.cpp
        src/Graphics/SwapChainImageCountPreference.cpp
        src/Graphics/SwapChainPresentModePreference.cpp
        src/Graphics/SwapChainVulkanSurfaceExtentPreference.cpp
        src/Graphics/SwapChainVulkanSurfaceFormatPreference.cpp
        src/Graphics/VulkanDeviceQueue.cpp
        src/Graphics/VulkanInstance.cpp
        src/Graphics/VulkanLogicalDevice.cpp
        src/Graphics/VulkanLogicalDeviceCreationStrategy.cpp
        src/Graphics/VulkanPhysicalDevice.cpp
        src/Graphics/VulkanPhysicalDeviceAPIVersionRequirements.cpp
        src/Graphics/VulkanPhysicalDeviceExtensionsRequirements.cpp
        src/Graphics/VulkanPhysicalDeviceFeatureRequirements.cpp
        src/Graphics/VulkanPhysicalDeviceQueueFamilyRequirements.cpp
        src/Graphics/VulkanPhysicalDeviceSelectionStrategy.cpp
        src/Graphics/VulkanSurface.cpp
        src/Graphics/VulkanSwapChain.cpp
        src/Graphics/VulkanSwapChainCreateStrategy.cpp
        src/WindowHelper/ApplicationWindow.cpp
        src/WindowHelper/WindowRequiredVulkanExtensionsProvider.cpp
        src/ResourceManagement/Borrowed.cpp
        src/ResourceManagement/OwnerShip.cpp
        src/ResourceManagement/ResourcePtrConcept.cpp
        src/WindowHelper/GlfwWindowSurfaceResourceAcquisition.cpp
        src/WindowHelper/GlfwWindowSurfaceResourceAcquisition.cpp
        src/Graphics/RequiredVulkanInstanceLayerProvider.cpp
        src/Graphics/VulkanInstanceReadModel.cpp
        include/Graphics/VulkanInstanceReadModel.h
)

# Tutorialターゲットのヘッダをインポートする
target_include_directories(Tutorial PRIVATE include)

# プロジェクトが依存するパッケージの検索と追加
option(ENABLE_CPP20_MODULE "Enable C++ 20 module support for Vulkan" OFF)
find_packages_project_depends()
bind_vulkan_librally_to(Tutorial)
bind_window_lib_to(Tutorial)
