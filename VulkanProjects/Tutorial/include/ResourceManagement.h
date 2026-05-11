//
// Created by 沖田大河 on 2026/04/19.
//

#ifndef TUTORIAL_RESOURCEMANAGEMENT_H
#define TUTORIAL_RESOURCEMANAGEMENT_H

#include "ResourceManagement/Borrowed.h"
#include "ResourceManagement/DynamicArray.h"
#include "ResourceManagement/Memory/Bytes.h"
#include "ResourceManagement/Memory/ContinuousMemoryBlockPool.h"
#include "ResourceManagement/OwnerShip.h"

namespace Tutorial {
    using ResourceManagement::Alignment;
    using ResourceManagement::Borrowed;
    using ResourceManagement::Bytes;
    using ResourceManagement::ContinuousMemoryBlockPool;
    using ResourceManagement::DynamicArray;
    using ResourceManagement::OwnerShip;
}

#endif //TUTORIAL_RESOURCEMANAGEMENT_H
