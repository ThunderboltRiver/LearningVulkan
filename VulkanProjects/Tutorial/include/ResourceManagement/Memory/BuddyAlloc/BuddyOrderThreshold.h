#ifndef TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDERTHRESHOLD_H
#define TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDERTHRESHOLD_H

#include <cstddef>

namespace Tutorial::ResourceManagement::Memory::BuddyAlloc {

    /** BuddyAllocatorが許容する最大order値。order値は 0 から BUDDY_ORDER_THRESHOLD までを使う。 */
    constexpr std::size_t BUDDY_ORDER_THRESHOLD = 32;

    /** order 0 を含めた、BuddyAllocatorが保持するorder別メタデータの要素数。 */
    constexpr std::size_t BUDDY_ORDER_COUNT = BUDDY_ORDER_THRESHOLD + 1;
}

#endif //TUTORIAL_RESOURCE_MANAGEMENT_MEMORY_BUDDYALLOC_BUDDYORDERTHRESHOLD_H
