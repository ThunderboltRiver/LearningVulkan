//
// Created by Copilot on 2026/02/15.
//

#ifndef TUTORIAL_LOGGER_H
#define TUTORIAL_LOGGER_H

#include <iostream>
#include <string>

namespace Tutorial::Debug {
    class Logger {
    public:
        // Logs a message to standard output only in debug builds
        static void log(const std::string& message) {
#ifndef NDEBUG
            std::cout << message << std::endl;
#endif
        }
    };
}

#endif //TUTORIAL_LOGGER_H
