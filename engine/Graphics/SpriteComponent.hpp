#pragma once

#include <raylib.h>

namespace x2d
{

struct alignas(16) SpriteComponent
{
    Texture2D texture;
};
static_assert(sizeof(SpriteComponent) <= 64);

} // namespace x2d
