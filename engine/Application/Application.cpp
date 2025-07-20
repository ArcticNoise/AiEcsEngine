#include "Application.hpp"
#include "Platform/TimeSystem.hpp"
#include "ECS/SystemManager.hpp"

namespace x2d
{

void Application::Update()
{
    m_StateMachine.Tick();
    auto& sm = m_World.GetSystemManager();
    sm.BuildViews(m_World);

    auto* timeSys = sm.GetSystem<TimeSystem>();
    if (timeSys != nullptr)
    {
        timeSys->Update({});
    }

    sm.ForEachSystemInPhase(ESystemPhase::eSystemPhase_Init, [](ISystem& sys, const View& view) { sys.Update(view); });

    while (timeSys != nullptr && timeSys->ShouldRunFixedUpdate())
    {
        sm.BuildViews(m_World);
        sm.ForEachSystemInPhase(ESystemPhase::eSystemPhase_FixedUpdate, [](ISystem& sys, const View& view) { sys.Update(view); });
        timeSys->AdvanceFixedUpdate();
    }

    sm.BuildViews(m_World);
    sm.ForEachSystemInPhase(ESystemPhase::eSystemPhase_Update, [](ISystem& sys, const View& view) { sys.Update(view); });

    sm.BuildViews(m_World);
    sm.ForEachSystemInPhase(ESystemPhase::eSystemPhase_LateUpdate, [](ISystem& sys, const View& view) { sys.Update(view); });

    sm.BuildViews(m_World);
    sm.ForEachSystemInPhase(ESystemPhase::eSystemPhase_Render, [](ISystem& sys, const View& view) { sys.Update(view); });

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

