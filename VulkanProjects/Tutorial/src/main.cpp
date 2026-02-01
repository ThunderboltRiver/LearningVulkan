#include <Application.h>
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        Tutorial::Application application;
        application.run();
    } catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
