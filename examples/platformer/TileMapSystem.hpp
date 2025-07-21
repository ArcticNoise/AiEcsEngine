#pragma once

#include <raylib.h>

#include "ECS/System.hpp"
#include "ECS/TransformComponent.hpp"
#include "examples/platformer/CameraFollowSystem.hpp"
#include "examples/platformer/Components.hpp"

namespace platformer {

class TileMapSystem final : public x2d::ISystem {
  public:
    explicit TileMapSystem(const CameraFollowSystem &camera);
    void Update(const x2d::View &view) override;

  private:
    const CameraFollowSystem &m_Camera;
};

} // namespace platformer
