#include "Application.hpp"

namespace x2d
{

void Application::Update()
{
    m_StateMachine.Tick();
    m_World.GetSystemManager().BuildViews(m_World);
    m_World.GetSystemManager().ForEachSystem([](ISystem& sys, const View& view) { sys.Update(view); });
    m_World.EndFrame();
}

void Application::ChangeState(EAppState state)
{
    m_StateMachine.ChangeState(state);
}

StateMachine& Application::GetStateMachine()
{
    return m_StateMachine;
}

World& Application::GetWorld()
{
    return m_World;
}

} // namespace x2d

