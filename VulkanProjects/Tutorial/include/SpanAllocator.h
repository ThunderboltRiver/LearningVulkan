//
// Created by 沖田大河 on 2026/02/26.
//

#ifndef TUTORIAL_SPANALLOCATOR_H
#define TUTORIAL_SPANALLOCATOR_H
#include "PlacementStackAllocator.h"

// Spanの割り当てに使用するアロケータを管理するクラス
struct SpanAllocator {
    inline static PlacementStackAllocator* allocator = nullptr;

    static PlacementStackAllocator *getAllocator();

    static void setAllocator(PlacementStackAllocator* pAllocator);

    static void resetAllocator() noexcept;
};

#endif //TUTORIAL_SPANALLOCATOR_H