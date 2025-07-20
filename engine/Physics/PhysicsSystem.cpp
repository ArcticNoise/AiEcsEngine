#include "PhysicsSystem.hpp"

#include "ECS/World.hpp"

namespace x2d {

namespace {
struct Candidate {
    Entity entity;
    TransformComponent *transform;
    ColliderAABBComponent *collider;
    RigidBody2DComponent *body;
};

bool Overlaps(const Candidate &a, const Candidate &b) {
    const float leftA = a.transform->x - a.collider->halfWidth;
    const float rightA = a.transform->x + a.collider->halfWidth;
    const float topA = a.transform->y - a.collider->halfHeight;
    const float bottomA = a.transform->y + a.collider->halfHeight;

    const float leftB = b.transform->x - b.collider->halfWidth;
    const float rightB = b.transform->x + b.collider->halfWidth;
    const float topB = b.transform->y - b.collider->halfHeight;
    const float bottomB = b.transform->y + b.collider->halfHeight;

    return leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB;
}
} // namespace

PhysicsSystem::PhysicsSystem(EventQueue<CollisionEvent> &collisions,
                             EventQueue<TriggerEvent> &triggers)
    : m_Collisions(collisions), m_Triggers(triggers) {}

void PhysicsSystem::Update(const View &view) {
    if (m_World == nullptr) {
        return;
    }

    auto *timeSys = m_World->GetSystemManager().GetSystem<TimeSystem>();
    const float dt = timeSys != nullptr ? timeSys->GetFixedDelta() : 1.0f / 60.0f;

    std::vector<Candidate> candidates;
    candidates.reserve(view.count);

    for (std::size_t i = 0; i < view.count; ++i) {
        const Entity e = view.entities[i];
        auto &tr = m_World->GetComponent<TransformComponent>(e);
        auto &col = m_World->GetComponent<ColliderAABBComponent>(e);
        auto &rb = m_World->GetComponent<RigidBody2DComponent>(e);
        if (!rb.isStatic) {
            tr.x += rb.velocity.x * dt;
            tr.y += rb.velocity.y * dt;
        }
        candidates.push_back({e, &tr, &col, &rb});
    }

    std::sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
        return (a.transform->x - a.collider->halfWidth) < (b.transform->x - b.collider->halfWidth);
    });

    for (std::size_t i = 0; i < candidates.size(); ++i) {
        const Candidate &a = candidates[i];
        const float rightA = a.transform->x + a.collider->halfWidth;
        for (std::size_t j = i + 1; j < candidates.size(); ++j) {
            const Candidate &b = candidates[j];
            if ((b.transform->x - b.collider->halfWidth) > rightA) {
                break;
            }
            if (Overlaps(a, b)) {
                if (a.collider->isTrigger || b.collider->isTrigger) {
                    m_Triggers.Enqueue({a.entity, b.entity});
                } else {
                    m_Collisions.Enqueue({a.entity, b.entity});
                }
            }
        }
    }
}

} // namespace x2d
