#pragma once

#include "state_machine.hpp"
#include "ECS/World.hpp"

namespace x2d
{

class Application
{
public:
    Application() = default;

    template<class TSystem, class... Args>
    TSystem& RegisterSystem(const Signature& signature, Args&&... args)
    {
        return m_World.GetSystemManager().RegisterSystem<TSystem>(signature, std::forward<Args>(args)...);
    }

    void Update();
    void ChangeState(EAppState state);

    StateMachine& GetStateMachine();
    World& GetWorld();

private:
    StateMachine m_StateMachine;
    World m_World;
};

} // namespace x2d

