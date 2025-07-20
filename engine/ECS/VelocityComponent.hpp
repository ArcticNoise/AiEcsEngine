#pragma once

namespace x2d
{

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable:4324)
#endif
struct alignas(16) VelocityComponent
{
    float vx;
    float vy;
};
#ifdef _MSC_VER
#    pragma warning(pop)
#endif
static_assert(sizeof(VelocityComponent) <= 64);

} // namespace x2d

