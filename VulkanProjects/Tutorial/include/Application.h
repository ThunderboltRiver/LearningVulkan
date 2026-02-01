//
// Created by 沖田大河 on 2026/01/31.
//

#ifndef TUTORIAL_APPLICATION_H
#define TUTORIAL_APPLICATION_H

namespace Tutorial
{
    struct Application {
    public:
        Application();

        /**
         * アプリケーションの実行
         */
        void run() {
            initializeGraphicsAPI();
            mainLoop();
            throwableResourceCleanup();
        }

        /**
         * アプリケーションをデストラクト
         * 基本的には不要になった解放に安全なリソースの解放を行う
         */
        virtual ~Application();
    private:

        /**
         * メインループの実行
         */
        void mainLoop();

        /**
         * グラフィックスAPIの初期化
         */
        void initializeGraphicsAPI();

        /**
         * 解放時に例外が発生し得るリソースの解放処理
         * RAIIによるデメリットとしての解放漏れや例外検知のために作成
         */
        void throwableResourceCleanup();
    };
}

#endif //TUTORIAL_APPLICATION_H