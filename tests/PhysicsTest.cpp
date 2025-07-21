#include <catch2/catch_test_macros.hpp>

#include "ECS/TransformComponent.hpp"
#include "ECS/World.hpp"
#include "Physics/ColliderAABBComponent.hpp"
#include "Physics/EventQueue.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Physics/RigidBody2DComponent.hpp"

TEST_CASE("EventQueue enqueue/dequeue", "[physics]") {
    x2d::EventQueue<x2d::CollisionEvent, 4> q;
    REQUIRE(q.IsEmpty());
    q.Enqueue({1, 2});
    x2d::CollisionEvent ev{};
    REQUIRE(q.Dequeue(ev));
    REQUIRE(ev.a == 1);
    REQUIRE(ev.b == 2);
}

TEST_CASE("PhysicsSystem detects collision", "[physics]") {
    x2d::World world;
    x2d::Signature sig;
    sig.set(x2d::ComponentTypeId<x2d::TransformComponent>());
    sig.set(x2d::ComponentTypeId<x2d::RigidBody2DComponent>());
    sig.set(x2d::ComponentTypeId<x2d::ColliderAABBComponent>());

    x2d::EventQueue<x2d::CollisionEvent> collisions;
    x2d::EventQueue<x2d::TriggerEvent> triggers;
    world.GetSystemManager().RegisterSystem<x2d::PhysicsSystem>(x2d::ESystemPhase::eSystemPhase_FixedUpdate,
                                                           sig, collisions, triggers);

    x2d::Entity a = world.CreateEntity();
    world.AddComponent<x2d::TransformComponent>(a, {0.0f, 0.0f, 0.0f, 1.0f});
    world.AddComponent<x2d::RigidBody2DComponent>(a, {});
    world.AddComponent<x2d::ColliderAABBComponent>(a, {0.5f, 0.5f, false});

    x2d::Entity b = world.CreateEntity();
    world.AddComponent<x2d::TransformComponent>(b, {0.4f, 0.0f, 0.0f, 1.0f});
    world.AddComponent<x2d::RigidBody2DComponent>(b, {});
    world.AddComponent<x2d::ColliderAABBComponent>(b, {0.5f, 0.5f, false});

    auto &sm = world.GetSystemManager();
    sm.BuildViews(world);
    sm.ForEachSystemInPhase(x2d::ESystemPhase::eSystemPhase_FixedUpdate,
                            [](x2d::ISystem &sys, const x2d::View &view) { sys.Update(view); });

    x2d::CollisionEvent evt{};
    REQUIRE(collisions.Dequeue(evt));
    REQUIRE(((evt.a == a && evt.b == b) || (evt.a == b && evt.b == a)));
}
