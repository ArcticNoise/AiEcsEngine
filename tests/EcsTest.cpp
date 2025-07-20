#include <catch2/catch_test_macros.hpp>
#include <chrono>

#include "engine/ECS/World.hpp"
#include "engine/ECS/TransformComponent.hpp"
#include "engine/ECS/VelocityComponent.hpp"
#include "engine/ECS/DamageEventComponent.hpp"
#include "engine/ECS/OneFrame.hpp"

using namespace x2d;

TEST_CASE("Entity creation/destruction performance", "[ecs]")
{
    World world;
    constexpr std::size_t count = 1'000'000;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Entity> entities;
    entities.reserve(count);
    for (std::size_t i = 0; i < count; ++i)
    {
        entities.push_back(world.CreateEntity());
    }
    for (auto e : entities)
    {
        world.DestroyEntity(e);
    }
    auto end = std::chrono::high_resolution_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    REQUIRE(ms < 50);
}

TEST_CASE("Signature updates", "[ecs]")
{
    World world;
    Entity e = world.CreateEntity();
    world.AddComponent<TransformComponent>(e, {});
    world.AddComponent<VelocityComponent>(e, {});
    const auto& sig = world.GetSignatures()[GetIndex(e)];
    REQUIRE(sig.test(ComponentTypeId<TransformComponent>()));
    REQUIRE(sig.test(ComponentTypeId<VelocityComponent>()));
}

TEST_CASE("Swap erase integrity", "[ecs]")
{
    World world;
    Entity e1 = world.CreateEntity();
    Entity e2 = world.CreateEntity();
    world.AddComponent<TransformComponent>(e1, {1.0f,2.0f,0.0f,1.0f});
    world.AddComponent<TransformComponent>(e2, {3.0f,4.0f,0.0f,1.0f});

    world.RemoveComponent<TransformComponent>(e1);
    REQUIRE_FALSE(world.HasComponent<TransformComponent>(e1));
    REQUIRE(world.HasComponent<TransformComponent>(e2));
}

TEST_CASE("OneFrame clearing", "[ecs]")
{
    World world;
    Entity e = world.CreateEntity();
    world.AddComponent<OneFrame<DamageEventComponent>>(e, {10});
    REQUIRE(world.HasComponent<OneFrame<DamageEventComponent>>(e));
    world.EndFrame();
    REQUIRE_FALSE(world.HasComponent<OneFrame<DamageEventComponent>>(e));
}

