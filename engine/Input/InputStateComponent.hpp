#pragma once

#include <raylib.h>

namespace x2d
{

struct alignas(16) InputStateComponent
{
    bool moveLeft;
    bool moveRight;
    bool moveUp;
    bool moveDown;
    bool action;
    Vector2 mousePosition;
};
static_assert(sizeof(InputStateComponent) <= 64);

} // namespace x2d
