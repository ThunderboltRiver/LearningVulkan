//
// Created by 沖田大河 on 2026/02/21.
//

#include "Span.h"

template<>
PlacementStackAllocator* Span<void*>::_allocator = nullptr;