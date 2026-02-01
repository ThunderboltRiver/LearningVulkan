//
// Created by 沖田大河 on 2026/01/31.
//

#include <iostream>
#include <Application.h>

namespace Tutorial
{
    Application::Application(){}

    void Application::initializeGraphicsAPI()
    {
        std::cout << "Tutorial Application initializeGraphicsAPI" << std::endl;
    }

    void Application::mainLoop()
    {
        std::cout << "main loop" << std::endl;
    }

    void Application::throwableResourceCleanup()
    {
        std::cout << "throwable resource cleanup" << std::endl;
    }

    Application::~Application()
    {
        std::cout << "destructor" << std::endl;
    }
}
