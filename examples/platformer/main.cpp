#include "Application/Application.hpp"
#include "ECS/TransformComponent.hpp"
#include "Input/InputStateComponent.hpp"
#include "Input/InputSystem.hpp"
#include "Platform/TimeSystem.hpp"
#include "Platform/WindowSystem.hpp"

#include "examples/platformer/CameraFollowSystem.hpp"
#include "examples/platformer/Components.hpp"
#include "examples/platformer/MovementSystem.hpp"
#include "examples/platformer/TileMapSystem.hpp"

int main()
{
    x2d::Application app;
    x2d::World& world = app.GetWorld();

    x2d::Signature windowSig{};
    app.RegisterSystem<x2d::WindowSystem>(x2d::ESystemPhase::eSystemPhase_Init,
                                          windowSig, 640, 480,
                                          std::string("Platformer"));

    x2d::Signature timeSig{};
    app.RegisterSystem<x2d::TimeSystem>(x2d::ESystemPhase::eSystemPhase_Init,
                                        timeSig);

    x2d::Signature inputSig;
    inputSig.set(x2d::ComponentTypeId<x2d::InputStateComponent>());
    app.RegisterSystem<x2d::InputSystem>(x2d::ESystemPhase::eSystemPhase_Update,
                                         inputSig);

    x2d::Signature moveSig;
    moveSig.set(x2d::ComponentTypeId<x2d::TransformComponent>());
    moveSig.set(x2d::ComponentTypeId<x2d::InputStateComponent>());
    moveSig.set(x2d::ComponentTypeId<platformer::PlayerComponent>());
    app.RegisterSystem<platformer::MovementSystem>(
        x2d::ESystemPhase::eSystemPhase_Update, moveSig);

    x2d::Signature camSig;
    camSig.set(x2d::ComponentTypeId<x2d::TransformComponent>());
    camSig.set(x2d::ComponentTypeId<platformer::CameraFollowComponent>());
    auto& camSys = app.RegisterSystem<platformer::CameraFollowSystem>(
        x2d::ESystemPhase::eSystemPhase_Update, camSig);

    x2d::Signature mapSig;
    mapSig.set(x2d::ComponentTypeId<x2d::TransformComponent>());
    mapSig.set(x2d::ComponentTypeId<platformer::TileMapComponent>());
    app.RegisterSystem<platformer::TileMapSystem>(
        x2d::ESystemPhase::eSystemPhase_Render, mapSig, camSys);

    x2d::Entity player = world.CreateEntity();
    world.AddComponent<x2d::TransformComponent>(player, {32.0f, 32.0f, 0.0f, 1.0f});
    world.AddComponent<x2d::InputStateComponent>(player, {});
    world.AddComponent<platformer::PlayerComponent>(player, {});
    world.AddComponent<platformer::CameraFollowComponent>(player, {0.0f, 0.0f});

    static const int mapWidth = 10;
    static const int mapHeight = 8;
    static const int mapData[mapWidth * mapHeight] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    x2d::Entity map = world.CreateEntity();
    world.AddComponent<x2d::TransformComponent>(map, {0.0f, 0.0f, 0.0f, 1.0f});
    world.AddComponent<platformer::TileMapComponent>(map,
                                                    {mapData, mapWidth, mapHeight, 32});

    app.ChangeState(x2d::EAppState::eAppState_MainMenu);

    int frame = 0;
    while (!WindowShouldClose() && frame < 240)
    {
        if (frame == 60)
        {
            app.ChangeState(x2d::EAppState::eAppState_Gameplay);
        }
        if (frame == 120)
        {
            app.ChangeState(x2d::EAppState::eAppState_Pause);
        }
        if (frame == 180)
        {
            app.ChangeState(x2d::EAppState::eAppState_Shutdown);
        }
        app.Update();
        ++frame;
    }

    return 0;
}
