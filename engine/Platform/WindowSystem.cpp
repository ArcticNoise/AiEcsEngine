#include "WindowSystem.hpp"

#include "ECS/World.hpp"

namespace x2d
{

WindowSystem::WindowSystem(int width, int height, std::string title)
{
    InitWindow(width, height, title.c_str());
    SetTargetFPS(60);
    m_Initialized = true;
}

WindowSystem::~WindowSystem()
{
    if (m_Initialized)
    {
        CloseWindow();
    }
}

void WindowSystem::Update(const View&)
{
    if (m_World == nullptr)
    {
        return;
    }
    if (IsWindowResized())
    {
        const int w = GetScreenWidth();
        const int h = GetScreenHeight();
        Entity e = m_World->CreateEntity();
        m_World->AddComponent<OneFrame<WindowResizeEventComponent>>(e, {w, h});
    }
}

} // namespace x2d
