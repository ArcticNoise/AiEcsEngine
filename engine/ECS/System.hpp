#pragma once

#include <vector>

#include "Entity.hpp"

namespace x2d
{

struct View
{
    const Entity* entities{nullptr};
    std::size_t count{0};
};

class World;

class ISystem
{
public:
    virtual ~ISystem() = default;
    virtual void Update(const View& view) = 0;
    virtual void SetWorld(World* world) { m_World = world; }

protected:
    World* m_World{nullptr};
};

} // namespace x2d

