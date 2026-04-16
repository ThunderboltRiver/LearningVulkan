//
// Created by 沖田大河 on 2026/04/16.
//

#ifndef TUTORIAL_RESOURCE_POINTER_CONCEPT_H
#define TUTORIAL_RESOURCE_POINTER_CONCEPT_H
#include <type_traits>

namespace Tutorial::ResourceManagement
{
    template <typename T>
    concept ResourcePtr = std::is_pointer_v<T>;
}


#endif //TUTORIAL_RESOURCE_POINTER_CONCEPT_H