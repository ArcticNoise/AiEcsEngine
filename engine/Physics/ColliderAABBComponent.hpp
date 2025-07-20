#pragma once

namespace x2d {

struct alignas(16) ColliderAABBComponent {
    float halfWidth;
    float halfHeight;
    bool isTrigger;
};
static_assert(sizeof(ColliderAABBComponent) <= 64);

} // namespace x2d
