#pragma once

namespace x2d {

struct alignas(16) VelocityComponent {
    float vx;
    float vy;
};
static_assert(sizeof(VelocityComponent) <= 64);

} // namespace x2d
