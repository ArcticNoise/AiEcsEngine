#pragma once

#include "ECS/System.hpp"
#include "ECS/TransformComponent.hpp"
#include "Input/InputStateComponent.hpp"
#include "examples/platformer/Components.hpp"

namespace platformer
{

class MovementSystem final : public x2d::ISystem
{
public:
    void Update(const x2d::View& view) override;
};

} // namespace platformer
