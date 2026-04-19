//
// Created by 沖田大河 on 2026/02/02.
//

#ifndef TUTORIAL_APPLICATIONWINDOW_H
#define TUTORIAL_APPLICATIONWINDOW_H

#include "Graphics/VulkanInstance.h"
#include "Graphics/VulkanSurface.h"
#include <GLFW/glfw3.h>

#include "Graphics/FrameBufferSize.h"

namespace Tutorial::WindowHelper {
    namespace rsm = Tutorial::ResourceManagement;

    class ApplicationWindow {
    public:
        ApplicationWindow(
            const uint32_t windowWidth,
            const uint32_t windowHeight,
            const char* const windowTitle):
            _windowPtr(initWindow(windowWidth, windowHeight, windowTitle)),
            _width(windowWidth),
            _height(windowHeight),
            _windowTitle(windowTitle)
        {};

        /**
         * GLFWwindowへの借用を返す
         * @return GLFWwindowへの借用
         */
        [[nodiscard]] rsm::Borrowed<GLFWwindow*> getHandler() const;

        /**
         * 閉じるべきか
         * @return 閉じるべきならtrue、そうでないならfalse
         */
        [[nodiscard]] bool shouldClose() const;

        /**
         *  このウィンドウのイベントキューに詰められたイベントを即座に処理する
         */
        void pollEvents() const;

        // コピー禁止。ウィンドウの所有権を持つのは一つのインスタンスのみにするため
        ApplicationWindow(const ApplicationWindow&) = delete;
        ApplicationWindow& operator=(const ApplicationWindow&) = delete;

        /**
         * このウィンドウのフレームバッファの幅と高さを取得する
         */
        [[nodiscard]] Graphics::FrameBufferSize getBufferSize() const;

        virtual ~ApplicationWindow();
    private:

        GLFWwindow* _windowPtr;
        const uint32_t _width;
        const uint32_t _height;
        const char* const _windowTitle;

        /**
         * ウィンドウを初期化する
         * @param windowWidth ウィンドウの幅
         * @param windowHeight ウィンドウの高さ
         * @param windowTitle ウィンドウのタイトル
         * @return ウィンドウへのポインタ
         */
        GLFWwindow* initWindow(uint32_t windowWidth, uint32_t windowHeight, const char* windowTitle);
    };

}

#endif //TUTORIAL_APPLICATIONWINDOW_H
