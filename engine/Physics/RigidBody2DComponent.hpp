#pragma once

#include <raylib.h>

namespace x2d {

struct alignas(16) RigidBody2DComponent {
    Vector2 velocity{};
    bool isStatic{false};
};
static_assert(sizeof(RigidBody2DComponent) <= 64);

} // namespace x2d
