#include "TileMapSystem.hpp"

#include "ECS/World.hpp"

namespace platformer
{

TileMapSystem::TileMapSystem(const CameraFollowSystem& camera) : m_Camera(camera)
{
}

void TileMapSystem::Update(const x2d::View& view)
{
    if (m_World == nullptr)
    {
        return;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(m_Camera.GetCamera());

    for (std::size_t i = 0; i < view.count; ++i)
    {
        const x2d::Entity e = view.entities[i];
        auto& map = m_World->GetComponent<TileMapComponent>(e);
        auto& transform = m_World->GetComponent<x2d::TransformComponent>(e);

        for (int y = 0; y < map.height; ++y)
        {
            for (int x = 0; x < map.width; ++x)
            {
                int tile = map.tiles[y * map.width + x];
                Color color = tile == 1 ? DARKGRAY : LIGHTGRAY;
                float drawX = transform.x + static_cast<float>(x * map.tileSize);
                float drawY = transform.y + static_cast<float>(y * map.tileSize);
                DrawRectangle(static_cast<int>(drawX), static_cast<int>(drawY), map.tileSize,
                              map.tileSize, color);
            }
        }
    }

    EndMode2D();

    EndDrawing();
}

} // namespace platformer
