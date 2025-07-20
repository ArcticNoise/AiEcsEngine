#pragma once

#include <cstdint>

namespace x2d
{

struct alignas(16) TransformComponent
{
    float x;
    float y;
    float rotation;
    float scale;
};
static_assert(sizeof(TransformComponent) <= 64);

} // namespace x2d

