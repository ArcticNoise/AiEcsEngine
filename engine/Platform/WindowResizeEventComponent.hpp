#pragma once

namespace x2d
{

struct alignas(16) WindowResizeEventComponent
{
    int width;
    int height;
};
static_assert(sizeof(WindowResizeEventComponent) <= 64);

} // namespace x2d
