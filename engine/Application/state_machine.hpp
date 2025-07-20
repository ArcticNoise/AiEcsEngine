#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace x2d
{

enum class EAppState
{
    eAppState_Boot,
    eAppState_Preload,
    eAppState_MainMenu,
    eAppState_Gameplay,
    eAppState_Pause,
    eAppState_Shutdown,
};

class IState
{
public:
    virtual ~IState() = default;
    virtual void Enter() {}
    virtual void Exit() {}
    virtual void Tick() {}
};

class BootState : public IState {};
class PreloadState : public IState {};
class MainMenuState : public IState {};
class GameplayState : public IState {};
class PauseState : public IState {};
class ShutdownState : public IState {};

class StateMachine
{
public:
    using StatePtr = std::unique_ptr<IState>;
    using StateFactory = std::function<StatePtr(EAppState)>;

    StateMachine();
    explicit StateMachine(StateFactory factory);

    void PushState(StatePtr state);
    void PopState();
    void ReplaceState(StatePtr state);
    void ChangeState(EAppState state);
    void Tick();

private:
    StatePtr CreateState(EAppState state);

    std::vector<StatePtr> m_Stack;
    StateFactory m_Factory;
};

} // namespace x2d

