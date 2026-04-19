//
// Created by 沖田大河 on 2026/04/17.
//

#ifndef TUTORIAL_GLFWWINDOWSURFACERESOURCEACQUISITION_H
#define TUTORIAL_GLFWWINDOWSURFACERESOURCEACQUISITION_H

#include "Graphics/IVkSurfaceKHRResourceAcquisition.h"
#include <GLFW/glfw3.h>
#include "ResourceManagement.h"

namespace Tutorial::WindowHelper {
    class GlfwWindowSurfaceResourceAcquisition : public Graphics::IVkSurfaceKHRResourceAcquisition {
        Borrowed<GLFWwindow*> _windowHandler;
        public:
        explicit GlfwWindowSurfaceResourceAcquisition(Borrowed<GLFWwindow*> windowHandler);

        [[nodiscard]] OwnerShip<VkSurfaceKHR> execute(Borrowed<VkInstance> instance) const override;
    };
}

#endif //TUTORIAL_GLFWWINDOWSURFACERESOURCEACQUISITION_H
