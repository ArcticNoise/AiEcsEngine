#include <catch2/catch_test_macros.hpp>

#include "ECS/TransformComponent.hpp"
#include "ECS/World.hpp"
#include "Physics/ColliderAABBComponent.hpp"
#include "Physics/EventQueue.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Physics/RigidBody2DComponent.hpp"

using namespace x2d;

TEST_CASE("EventQueue enqueue/dequeue", "[physics]") {
    EventQueue<CollisionEvent, 4> q;
    REQUIRE(q.IsEmpty());
    q.Enqueue({1, 2});
    CollisionEvent ev{};
    REQUIRE(q.Dequeue(ev));
    REQUIRE(ev.a == 1);
    REQUIRE(ev.b == 2);
}

TEST_CASE("PhysicsSystem detects collision", "[physics]") {
    World world;
    Signature sig;
    sig.set(ComponentTypeId<TransformComponent>());
    sig.set(ComponentTypeId<RigidBody2DComponent>());
    sig.set(ComponentTypeId<ColliderAABBComponent>());

    EventQueue<CollisionEvent> collisions;
    EventQueue<TriggerEvent> triggers;
    world.GetSystemManager().RegisterSystem<PhysicsSystem>(ESystemPhase::eSystemPhase_FixedUpdate,
                                                           sig, collisions, triggers);

    Entity a = world.CreateEntity();
    world.AddComponent<TransformComponent>(a, {0.0f, 0.0f, 0.0f, 1.0f});
    world.AddComponent<RigidBody2DComponent>(a, {});
    world.AddComponent<ColliderAABBComponent>(a, {0.5f, 0.5f, false});

    Entity b = world.CreateEntity();
    world.AddComponent<TransformComponent>(b, {0.4f, 0.0f, 0.0f, 1.0f});
    world.AddComponent<RigidBody2DComponent>(b, {});
    world.AddComponent<ColliderAABBComponent>(b, {0.5f, 0.5f, false});

    auto &sm = world.GetSystemManager();
    sm.BuildViews(world);
    sm.ForEachSystemInPhase(ESystemPhase::eSystemPhase_FixedUpdate,
                            [](ISystem &sys, const View &view) { sys.Update(view); });

    CollisionEvent evt{};
    REQUIRE(collisions.Dequeue(evt));
    REQUIRE(((evt.a == a && evt.b == b) || (evt.a == b && evt.b == a)));
}
