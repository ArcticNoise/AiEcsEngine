#pragma once

#include <memory>
#include <vector>

#include "Signature.hpp"
#include "System.hpp"
#include "WorldFwd.hpp"

namespace x2d
{

class SystemManager final
{
public:
    template<class TSystem, class... Args>
    TSystem& RegisterSystem(const Signature& signature, Args&&... args)
    {
        auto sys = std::make_unique<TSystem>(std::forward<Args>(args)...);
        m_Systems.push_back({std::move(sys), signature, {}, {}});
        return *static_cast<TSystem*>(m_Systems.back().system.get());
    }

    void BuildViews(const World& world);

    template<class Func>
    void ForEachSystem(Func func)
    {
        for (auto& entry : m_Systems)
        {
            func(*entry.system, entry.view);
        }
    }

private:
    struct Entry
    {
        std::unique_ptr<ISystem> system;
        Signature signature;
        View view;
        std::vector<Entity> entityBuffer;
    };

    std::vector<Entry> m_Systems;
};

} // namespace x2d

