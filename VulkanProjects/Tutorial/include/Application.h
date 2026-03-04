//
// Created by 沖田大河 on 2026/01/31.
//

#ifndef TUTORIAL_APPLICATION_H
#define TUTORIAL_APPLICATION_H

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "ApplicationWindow.h"

namespace Tutorial
{
    struct Application {
    public:
        Application();

        /**
         * アプリケーションの実行
         */
        void run();

        /**
         * アプリケーションをデストラクト
         * 基本的には不要になった解放に安全なリソースの解放を行う
         */
        virtual ~Application();
    private:
        static constexpr uint32_t WINDOW_WIDTH = 800;
        static constexpr uint32_t WINDOW_HEIGHT = 720;
        static constexpr const char* const WINDOW_TITLE = "Tutorial";
        static constexpr VkApplicationInfo appInfo {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "Tutorial",
            .applicationVersion =  VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "No Engine",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_4
        };

        /**
         * メインループの実行
         */
        void mainLoop(const WindowHelper::ApplicationWindow& applicationWindow);

        /**
         * 解放時に例外が発生し得るリソースの解放処理
         * RAIIによるデメリットとしての解放漏れや例外検知のために作成
         */
        void throwableResourceCleanup();
    };
}

#endif //TUTORIAL_APPLICATION_H