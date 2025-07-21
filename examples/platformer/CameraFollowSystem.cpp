#include "CameraFollowSystem.hpp"

#include "ECS/World.hpp"

namespace platformer
{

const Camera2D& CameraFollowSystem::GetCamera() const
{
    return m_Camera;
}

void CameraFollowSystem::Update(const x2d::View& view)
{
    if (m_World == nullptr || view.count == 0)
    {
        return;
    }

    const x2d::Entity e = view.entities[0];
    auto& transform = m_World->GetComponent<x2d::TransformComponent>(e);
    auto& follow = m_World->GetComponent<CameraFollowComponent>(e);

    m_Camera.target = {transform.x + follow.offsetX, transform.y + follow.offsetY};
    m_Camera.offset = {static_cast<float>(GetScreenWidth()) * 0.5f,
                       static_cast<float>(GetScreenHeight()) * 0.5f};
    m_Camera.zoom = 1.0f;
}

} // namespace platformer
