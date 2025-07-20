#pragma once

#include <raylib.h>

namespace x2d
{

struct alignas(16) InputStateComponent
{
    bool moveLeft{false};
    bool moveRight{false};
    bool moveUp{false};
    bool moveDown{false};
    bool action{false};
    Vector2 mousePosition{0.0f, 0.0f};
};
static_assert(sizeof(InputStateComponent) <= 64);

} // namespace x2d
