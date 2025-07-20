#pragma once

#include <memory>
#include <vector>

#include "Signature.hpp"
#include "System.hpp"
#include "WorldFwd.hpp"

namespace x2d
{

enum class ESystemPhase
{
    eSystemPhase_Init,
    eSystemPhase_FixedUpdate,
    eSystemPhase_Update,
    eSystemPhase_LateUpdate,
    eSystemPhase_Render
};

class SystemManager final
{
public:
    template<class TSystem, class... Args>
    TSystem& RegisterSystem(ESystemPhase phase, const Signature& signature, Args&&... args)
    {
        auto sys = std::make_unique<TSystem>(std::forward<Args>(args)...);
        m_Systems.push_back({std::move(sys), signature, {}, {}, phase});
        return *static_cast<TSystem*>(m_Systems.back().system.get());
    }

    void BuildViews(const World& world);

    template<class TSystem>
    TSystem* GetSystem()
    {
        for (auto& entry : m_Systems)
        {
            if (auto* ptr = dynamic_cast<TSystem*>(entry.system.get()))
            {
                return ptr;
            }
        }
        return nullptr;
    }

    template<class Func>
    void ForEachSystem(Func func)
    {
        for (auto& entry : m_Systems)
        {
            func(*entry.system, entry.view);
        }
    }

    template<class Func>
    void ForEachSystemInPhase(ESystemPhase phase, Func func)
    {
        for (auto& entry : m_Systems)
        {
            if (entry.phase == phase)
            {
                func(*entry.system, entry.view);
            }
        }
    }

private:
    struct Entry
    {
        std::unique_ptr<ISystem> system;
        Signature signature;
        View view;
        std::vector<Entity> entityBuffer;
        ESystemPhase phase;
    };

    std::vector<Entry> m_Systems;
    World* m_World{nullptr};
};

} // namespace x2d

