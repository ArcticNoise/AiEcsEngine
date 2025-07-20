#pragma once

#include <raylib.h>

#include "ECS/System.hpp"
#include "Input/InputStateComponent.hpp"

namespace x2d
{

class InputSystem final : public ISystem
{
public:
    void Update(const View& view) override;
};

} // namespace x2d
