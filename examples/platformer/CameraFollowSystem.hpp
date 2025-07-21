#pragma once

#include <raylib.h>

#include "ECS/System.hpp"
#include "ECS/TransformComponent.hpp"
#include "examples/platformer/Components.hpp"

namespace platformer {

class CameraFollowSystem final : public x2d::ISystem {
  public:
    [[nodiscard]] const Camera2D &GetCamera() const;
    void Update(const x2d::View &view) override;

  private:
    Camera2D m_Camera{};
};

} // namespace platformer
