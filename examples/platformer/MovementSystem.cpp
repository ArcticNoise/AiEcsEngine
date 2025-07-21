#include "MovementSystem.hpp"

#include "ECS/World.hpp"

namespace platformer {

void MovementSystem::Update(const x2d::View &view) {
    if (m_World == nullptr) {
        return;
    }
    for (std::size_t i = 0; i < view.count; ++i) {
        const x2d::Entity e = view.entities[i];
        auto &transform = m_World->GetComponent<x2d::TransformComponent>(e);
        auto &input = m_World->GetComponent<x2d::InputStateComponent>(e);
        const float speed = 2.0f;
        if (input.moveLeft) {
            transform.x -= speed;
        }
        if (input.moveRight) {
            transform.x += speed;
        }
        if (input.moveUp) {
            transform.y -= speed;
        }
        if (input.moveDown) {
            transform.y += speed;
        }
    }
}

} // namespace platformer
