#pragma once

namespace x2d
{

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable:4324)
#endif
struct alignas(16) DamageEventComponent
{
    int amount;
};
#ifdef _MSC_VER
#    pragma warning(pop)
#endif
static_assert(sizeof(DamageEventComponent) <= 64);

} // namespace x2d

