//
// Created by 沖田大河 on 2026/04/17.
//

#ifndef TUTORIAL_GLFWWINDOWSURFACERESOURCEACQUISITION_H
#define TUTORIAL_GLFWWINDOWSURFACERESOURCEACQUISITION_H

#include "Graphics/IVkSurfaceKHRResourceAcquisition.h"
#include <GLFW/glfw3.h>

namespace Tutorial::WindowHelper {
    namespace RM = Tutorial::ResourceManagement;

    class GlfwWindowSurfaceResourceAcquisition : public Graphics::IVkSurfaceKHRResourceAcquisition {
        RM::Borrowed<GLFWwindow*> _windowHandler;
        public:
        explicit GlfwWindowSurfaceResourceAcquisition(RM::Borrowed<GLFWwindow*> windowHandler);

        [[nodiscard]] RM::OwnerShip<VkSurfaceKHR> execute(RM::Borrowed<VkInstance> instance) const override;
    };
}

#endif //TUTORIAL_GLFWWINDOWSURFACERESOURCEACQUISITION_H
