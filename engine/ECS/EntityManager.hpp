#pragma once

#include <vector>

#include "Entity.hpp"

namespace x2d
{

class EntityManager final
{
public:
    EntityManager() = default;

    [[nodiscard]] Entity CreateEntity();
    void DestroyEntity(Entity entity);
    [[nodiscard]] bool IsAlive(Entity entity) const;
    [[nodiscard]] std::size_t AliveCount() const;

    [[nodiscard]] std::uint16_t GetGenerationForIndex(std::uint32_t index) const;

private:
    std::vector<std::uint16_t> m_Generations;
    std::vector<std::uint32_t> m_FreeList;
    std::size_t m_Alive{0};
};

} // namespace x2d

