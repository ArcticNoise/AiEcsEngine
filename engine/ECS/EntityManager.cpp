#include "EntityManager.hpp"

namespace x2d
{

Entity EntityManager::CreateEntity()
{
    std::uint32_t index;
    if (!m_FreeList.empty())
    {
        index = m_FreeList.back();
        m_FreeList.pop_back();
    }
    else
    {
        index = static_cast<std::uint32_t>(m_Generations.size());
        m_Generations.push_back(0);
    }

    ++m_Alive;
    const std::uint16_t generation = m_Generations[index];
    return MakeEntity(index, generation);
}

void EntityManager::DestroyEntity(Entity entity)
{
    const std::uint32_t index = GetIndex(entity);
    if (index >= m_Generations.size())
    {
        return;
    }

    ++m_Generations[index];
    m_Generations[index] &= s_GenerationMask;
    m_FreeList.push_back(index);
    --m_Alive;
}

bool EntityManager::IsAlive(Entity entity) const
{
    const std::uint32_t index = GetIndex(entity);
    const std::uint16_t generation = GetGeneration(entity);
    return index < m_Generations.size() && m_Generations[index] == generation;
}

std::size_t EntityManager::AliveCount() const
{
    return m_Alive;
}

std::uint16_t EntityManager::GetGenerationForIndex(std::uint32_t index) const
{
    if (index < m_Generations.size())
    {
        return m_Generations[index];
    }
    return 0;
}

} // namespace x2d

