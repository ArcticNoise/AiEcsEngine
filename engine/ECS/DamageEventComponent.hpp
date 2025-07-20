#pragma once

namespace x2d {

struct alignas(16) DamageEventComponent {
    int amount;
};
static_assert(sizeof(DamageEventComponent) <= 64);

} // namespace x2d
