#pragma once

#include <unordered_map>
#include <vector>
#include <raylib.h>

#include "ECS/System.hpp"
#include "ECS/TransformComponent.hpp"
#include "Graphics/SpriteComponent.hpp"

namespace x2d
{

class RenderSystem2D final : public ISystem
{
public:
    void Update(const View& view) override;
};

} // namespace x2d
