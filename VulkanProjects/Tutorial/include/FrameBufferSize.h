//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_FRAME_BUFFER_SIZE_H
#define TUTORIAL_FRAME_BUFFER_SIZE_H

#include "Pixel.h"

namespace Tutorial::Graphics {

    // フレームバッファのサイズを表す構造体
    struct FrameBufferSize {
        const Pixel width;
        const Pixel height;

        explicit FrameBufferSize(Pixel width, Pixel height);

        uint32_t getWidthValue() const;
        uint32_t getHeightValue() const;
    };
};

#endif //TUTORIAL_FRAME_BUFFER_SIZE_H