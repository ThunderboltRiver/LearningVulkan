//
// Created by 沖田大河 on 2026/04/17.
//

#ifndef TUTORIAL_GLFWWINDOWSURFACERESOURCEACQUISITION_H
#define TUTORIAL_GLFWWINDOWSURFACERESOURCEACQUISITION_H

#include "Graphics/IVkSurfaceKHRResourceAcquisition.h"
#include <GLFW/glfw3.h>

namespace Tutorial::WindowHelper {
    namespace rsm = Tutorial::ResourceManagement;

    class GlfwWindowSurfaceResourceAcquisition : public Graphics::IVkSurfaceKHRResourceAcquisition {
        rsm::Borrowed<GLFWwindow*> _windowHandler;
        public:
        explicit GlfwWindowSurfaceResourceAcquisition(rsm::Borrowed<GLFWwindow*> windowHandler);

        [[nodiscard]] rsm::OwnerShip<VkSurfaceKHR> execute(rsm::Borrowed<VkInstance> instance) const override;
    };
}

#endif //TUTORIAL_GLFWWINDOWSURFACERESOURCEACQUISITION_H
