#include "RenderSystem2D.hpp"

#include <unordered_map>

#include "ECS/World.hpp"

namespace x2d
{

void RenderSystem2D::Update(const View& view)
{
    if (m_World == nullptr || view.count == 0)
    {
        return;
    }

    std::unordered_map<Texture2D*, std::vector<Entity>> batches;
    batches.reserve(view.count);
    for (std::size_t i = 0; i < view.count; ++i)
    {
        const Entity e = view.entities[i];
        auto& sprite = m_World->GetComponent<SpriteComponent>(e);
        batches[&sprite.texture].push_back(e);
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (auto& [texture, entities] : batches)
    {
        for (Entity e : entities)
        {
            auto& sprite = m_World->GetComponent<SpriteComponent>(e);
            auto& transform = m_World->GetComponent<TransformComponent>(e);
            Vector2 pos{transform.x, transform.y};
            DrawTextureEx(sprite.texture, pos, transform.rotation, transform.scale, WHITE);
        }
    }

    EndDrawing();
}

} // namespace x2d
