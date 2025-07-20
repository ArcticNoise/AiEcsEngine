#pragma once

#include <vector>

#include "ECS/System.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/ColliderAABBComponent.hpp"
#include "Physics/CollisionEvent.hpp"
#include "Physics/EventQueue.hpp"
#include "Physics/RigidBody2DComponent.hpp"
#include "Physics/TriggerEvent.hpp"
#include "Platform/TimeSystem.hpp"

namespace x2d {

class PhysicsSystem final : public ISystem {
  public:
    PhysicsSystem(EventQueue<CollisionEvent> &collisions, EventQueue<TriggerEvent> &triggers);

    void Update(const View &view) override;

  private:
    EventQueue<CollisionEvent> &m_Collisions;
    EventQueue<TriggerEvent> &m_Triggers;
};

} // namespace x2d
