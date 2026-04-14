//
// Created by 沖田大河 on 2026/04/11.
//

#ifndef TUTORIAL_PIXEL_H
#define TUTORIAL_PIXEL_H

#include <cstdint>

namespace Tutorial::Graphics {
    // ピクセルを表す構造体
    struct Pixel {
        const int32_t value;
        explicit Pixel(int32_t value);
    };
}


#endif //TUTORIAL_PIXEL_H