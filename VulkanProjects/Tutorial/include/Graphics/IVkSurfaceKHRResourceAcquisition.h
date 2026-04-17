//
// Created by 沖田大河 on 2026/04/17.
//

#ifndef TUTORIAL_IVKSURFACE_KHR_FACTORY_H
#define TUTORIAL_IVKSURFACE_KHR_FACTORY_H

#include <vulkan/vulkan.h>
#include "ResourceManagement/OwnerShip.h"
#include "ResourceManagement/Borrowed.h"

using Tutorial::ResourceManagement::OwnerShip;
using Tutorial::ResourceManagement::Borrowed;

namespace Tutorial::Graphics {

    /**
     * VkSurfaceKHRのリソースを獲得するためのインターフェース
     */
    class IVkSurfaceKHRResourceAcquisition {

    public:
        virtual ~IVkSurfaceKHRResourceAcquisition() = default;

        /**
         *　VkSurfaceKHRのリソース獲得を実行する
         * @param instance vkInstanceへの借用。VkSurfaceKHRの作成に使用される
         * @return VkSurfaceKHRの所有権
         */
        [[nodiscard]] virtual OwnerShip<VkSurfaceKHR> execute(Borrowed<VkInstance> instance) const = 0;
    };
}


#endif //TUTORIAL_IVKSURFACE_KHR_FACTORY_H