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
add_executable(Tutorial src/main.cpp
        src/Application.cpp
        src/ApplicationWindow.cpp)

# Tutorialターゲットのヘッダをインポートする
target_include_directories(Tutorial PRIVATE include)

# プロジェクトが依存するパッケージの検索と追加
option(ENABLE_CPP20_MODULE "Enable C++ 20 module support for Vulkan" OFF)
find_packages_project_depends()
bind_vulkan_librally_to(Tutorial)
bind_window_lib_to(Tutorial)
