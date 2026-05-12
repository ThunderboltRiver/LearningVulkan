//
// Created by 沖田大河 on 2026/02/26.
//

#ifndef TUTORIAL_SPANALLOCATOR_H
#define TUTORIAL_SPANALLOCATOR_H

#include "ResourceManagement/Memory/ContinuousMemoryBlockPool.h"

// Spanの割り当てに使用するアロケータを管理するクラス
struct SpanAllocator {
    inline static Tutorial::ResourceManagement::Memory::ContinuousMemoryBlockPool* allocator = nullptr;

    static Tutorial::ResourceManagement::Memory::ContinuousMemoryBlockPool* getAllocator();

    static void setAllocator(Tutorial::ResourceManagement::Memory::ContinuousMemoryBlockPool* pAllocator);

    static void resetAllocator() noexcept;
};

#endif //TUTORIAL_SPANALLOCATOR_H
