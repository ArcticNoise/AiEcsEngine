#pragma once

#include <cstddef>

namespace platformer {

struct alignas(16) PlayerComponent {};
static_assert(sizeof(PlayerComponent) <= 64);

struct alignas(16) CameraFollowComponent {
    float offsetX;
    float offsetY;
};
static_assert(sizeof(CameraFollowComponent) <= 64);

struct alignas(16) TileMapComponent {
    const int *tiles;
    int width;
    int height;
    int tileSize;
};
static_assert(sizeof(TileMapComponent) <= 64);

} // namespace platformer
