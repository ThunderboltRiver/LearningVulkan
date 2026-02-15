//
// Created by 沖田大河 on 2026/02/02.
//

#ifndef TUTORIAL_APPLICATIONWINDOW_H
#define TUTORIAL_APPLICATIONWINDOW_H
#include <GLFW/glfw3.h>

namespace Tutorial::WindowHelper {
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
         * 閉じるべきか
         * @return
         */
        bool shouldClose() const;

        /**
         *  このウィンドウのイベントキューに詰められたイベントを即座に処理する
         */
        void pollEvents() const;

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