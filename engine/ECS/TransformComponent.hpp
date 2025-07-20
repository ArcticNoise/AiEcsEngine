#pragma once

#include <cstdint>

namespace x2d
{

struct alignas(16) TransformComponent
{
    float x{0.0f};
    float y{0.0f};
    float rotation{0.0f};
    float scale{1.0f};
};
static_assert(sizeof(TransformComponent) <= 64);

} // namespace x2d

