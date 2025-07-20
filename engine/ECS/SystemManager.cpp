#include "SystemManager.hpp"

#include "World.hpp"

namespace x2d
{

void SystemManager::BuildViews(const World& world)
{
    for (auto& entry : m_Systems)
    {
        entry.entityBuffer.clear();
        const auto& signatures = world.GetSignatures();
        entry.entityBuffer.reserve(signatures.size());
        for (std::size_t i = 0; i < signatures.size(); ++i)
        {
            const Signature& sig = signatures[i];
            if ((sig & entry.signature) == entry.signature)
            {
                const std::uint16_t gen = world.GetGenerationForIndex(static_cast<std::uint32_t>(i));
                entry.entityBuffer.push_back(MakeEntity(static_cast<std::uint32_t>(i), gen));
            }
        }
        entry.view.entities = entry.entityBuffer.data();
        entry.view.count = entry.entityBuffer.size();
    }
}

} // namespace x2d

