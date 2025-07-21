#include <catch2/catch_test_macros.hpp>
#include <chrono>

#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/VelocityComponent.hpp"
#include "ECS/DamageEventComponent.hpp"
#include "ECS/OneFrame.hpp"

TEST_CASE("Entity creation/destruction performance", "[ecs]")
{
    x2d::World world;
    constexpr std::size_t count = 1'000'000;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<x2d::Entity> entities;
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
    REQUIRE(ms < 200);
}

TEST_CASE("Signature updates", "[ecs]")
{
    x2d::World world;
    x2d::Entity e = world.CreateEntity();
    world.AddComponent<x2d::TransformComponent>(e, {});
    world.AddComponent<x2d::VelocityComponent>(e, {});
    const auto& sig = world.GetSignatures()[x2d::GetIndex(e)];
    REQUIRE(sig.test(x2d::ComponentTypeId<x2d::TransformComponent>()));
    REQUIRE(sig.test(x2d::ComponentTypeId<x2d::VelocityComponent>()));
}

TEST_CASE("Swap erase integrity", "[ecs]")
{
    x2d::World world;
    x2d::Entity e1 = world.CreateEntity();
    x2d::Entity e2 = world.CreateEntity();
    world.AddComponent<x2d::TransformComponent>(e1, {1.0f,2.0f,0.0f,1.0f});
    world.AddComponent<x2d::TransformComponent>(e2, {3.0f,4.0f,0.0f,1.0f});

    world.RemoveComponent<x2d::TransformComponent>(e1);
    REQUIRE_FALSE(world.HasComponent<x2d::TransformComponent>(e1));
    REQUIRE(world.HasComponent<x2d::TransformComponent>(e2));
}

TEST_CASE("OneFrame clearing", "[ecs]")
{
    x2d::World world;
    x2d::Entity e = world.CreateEntity();
    world.AddComponent<x2d::OneFrame<x2d::DamageEventComponent>>(e, {10});
    REQUIRE(world.HasComponent<x2d::OneFrame<x2d::DamageEventComponent>>(e));
    world.EndFrame();
    REQUIRE_FALSE(world.HasComponent<x2d::OneFrame<x2d::DamageEventComponent>>(e));
}

