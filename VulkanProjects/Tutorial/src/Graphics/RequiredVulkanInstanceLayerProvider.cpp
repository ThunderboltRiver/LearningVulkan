//
// Created by 沖田大河 on 2026/04/18.
//

#include "Graphics/RequiredVulkanInstanceLayerProvider.h"

#include <iterator>

namespace Tutorial::Graphics {

    Span<char const *> RequiredVulkanInstanceLayerProvider::getLayerNames() const {
        if constexpr (enableValidationLayers) {
            // バリデーションレイヤーを有効にする場合は、VK_LAYER_KHRONOS_validationレイヤーを要求する
            auto result = Span<char const*>::stackAlloc(1);
            result.Add("VK_LAYER_KHRONOS_validation");
            return result;
        }
        // バリデーションレイヤーを有効にしない場合は、空の配列を返す
        return Span<char const *>::createEmpty();
    }
}
