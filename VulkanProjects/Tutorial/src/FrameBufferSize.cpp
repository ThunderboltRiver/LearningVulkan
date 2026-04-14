//
// Created by 沖田大河 on 2026/04/11.
//

#include "FrameBufferSize.h"

#include <assert.h>
#include <stdexcept>

namespace Tutorial::Graphics {
    FrameBufferSize::FrameBufferSize(Pixel width, Pixel height): width(width), height(height)
    {
        assert(width.value > 0);
        assert(height.value > 0);
    }

    uint32_t FrameBufferSize::getWidthValue() const {
        return static_cast<uint32_t>(width.value);
    }

    uint32_t FrameBufferSize::getHeightValue() const {
        return static_cast<uint32_t>(height.value);
    }
}
