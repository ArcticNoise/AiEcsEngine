#include "InputSystem.hpp"

#include "ECS/World.hpp"

namespace x2d
{

void InputSystem::Update(const View& view)
{
    if (m_World == nullptr)
    {
        return;
    }
    for (std::size_t i = 0; i < view.count; ++i)
    {
        const Entity e = view.entities[i];
        auto& state = m_World->GetComponent<InputStateComponent>(e);
        state.moveLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
        state.moveRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
        state.moveUp = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
        state.moveDown = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
        state.action = IsKeyDown(KEY_SPACE);
        state.mousePosition = GetMousePosition();
    }
}

} // namespace x2d
