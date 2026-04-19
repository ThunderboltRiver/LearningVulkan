//
// Created by 沖田大河 on 2026/04/18.
//

#include "Graphics/VulkanInstanceReadModel.h"

#include "Graphics/VulkanPhysicalDevice.h"

namespace Tutorial::Graphics {
    namespace rsm = Tutorial::ResourceManagement;

    VulkanInstanceReadModel::VulkanInstanceReadModel(rsm::Borrowed<VkInstance> vkInstance):
        _vkInstance(vkInstance) {
    }

    Span<VulkanPhysicalDevice> VulkanInstanceReadModel::enumeratePhysicalDevices() const {
        uint32_t physicalDeviceCount = 0;
        if (vkEnumeratePhysicalDevices(_vkInstance.getRawHandle(), &physicalDeviceCount, nullptr) != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate physical devices count");
        }
        // この関数のスコープを抜けた後のSpanの解放順序が、ただしくLIFOになるよう先に返却用の配列を確保する
        // あくまで応急処置になるので根本的には配列のアロケータを見直す必要がある
        auto results = Span<VulkanPhysicalDevice>::stackAlloc(physicalDeviceCount);
        auto physicalDevices = Span<VkPhysicalDevice>::stackAlloc(physicalDeviceCount);
        if (vkEnumeratePhysicalDevices(_vkInstance.getRawHandle(), &physicalDeviceCount, physicalDevices.getHeadPtr()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to enumerate physical devices");
        }
        physicalDevices.markFilled(physicalDeviceCount);
        for (auto physicalDevice: physicalDevices) {
            results.Add(VulkanPhysicalDevice(rsm::Borrowed(physicalDevice)));
        }
        return results;
    }
}
