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

class ISystem
{
public:
    virtual ~ISystem() = default;
    virtual void Update(const View& view) = 0;
};

} // namespace x2d

