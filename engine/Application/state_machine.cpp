#include "state_machine.hpp"

namespace x2d
{

StateMachine::StateMachine() : StateMachine(StateFactory{}) {}

StateMachine::StateMachine(StateFactory factory)
    : m_Factory(std::move(factory))
{
}

void StateMachine::PushState(StatePtr state)
{
    if (state == nullptr)
    {
        return;
    }
    state->Enter();
    m_Stack.push_back(std::move(state));
}

void StateMachine::PopState()
{
    if (m_Stack.empty())
    {
        return;
    }
    m_Stack.back()->Exit();
    m_Stack.pop_back();
}

void StateMachine::ReplaceState(StatePtr state)
{
    PopState();
    PushState(std::move(state));
}

void StateMachine::ChangeState(EAppState state)
{
    ReplaceState(CreateState(state));
}

void StateMachine::Tick()
{
    if (!m_Stack.empty())
    {
        m_Stack.back()->Tick();
    }
}

StateMachine::StatePtr StateMachine::CreateState(EAppState state)
{
    if (m_Factory)
    {
        return m_Factory(state);
    }

    switch (state)
    {
    case EAppState::eAppState_Boot:
        return std::make_unique<BootState>();
    case EAppState::eAppState_Preload:
        return std::make_unique<PreloadState>();
    case EAppState::eAppState_MainMenu:
        return std::make_unique<MainMenuState>();
    case EAppState::eAppState_Gameplay:
        return std::make_unique<GameplayState>();
    case EAppState::eAppState_Pause:
        return std::make_unique<PauseState>();
    case EAppState::eAppState_Shutdown:
    default:
        return std::make_unique<ShutdownState>();
    }
}

} // namespace x2d

