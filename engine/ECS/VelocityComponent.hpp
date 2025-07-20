#pragma once

namespace x2d
{

struct alignas(16) VelocityComponent
{
    float vx{0.0f};
    float vy{0.0f};
};
static_assert(sizeof(VelocityComponent) <= 64);

} // namespace x2d

