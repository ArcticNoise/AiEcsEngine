#include "World.hpp"

namespace x2d
{

World::World() = default;

Entity World::CreateEntity()
{
    const Entity entity = m_EntityManager.CreateEntity();
    const std::uint32_t index = GetIndex(entity);
    if (m_Signatures.size() <= index)
    {
        m_Signatures.resize(index + 1);
    }
    m_Signatures[index].reset();
    return entity;
}

void World::DestroyEntity(Entity entity)
{
    m_EntityManager.DestroyEntity(entity);
    const std::uint32_t index = GetIndex(entity);
    if (index < m_Signatures.size())
    {
        m_Signatures[index].reset();
    }
    for (auto& [id, storage] : m_Storages)
    {
        storage->RemoveEntity(entity);
    }
}

const std::vector<Signature>& World::GetSignatures() const
{
    return m_Signatures;
}

std::uint16_t World::GetGenerationForIndex(std::uint32_t index) const
{
    return m_EntityManager.GetGenerationForIndex(index);
}

SystemManager& World::GetSystemManager()
{
    return m_SystemManager;
}

const SystemManager& World::GetSystemManager() const
{
    return m_SystemManager;
}

void World::EndFrame()
{
    for (auto& [id, storage] : m_Storages)
    {
        storage->ClearIfOneFrame();
    }
}

} // namespace x2d

