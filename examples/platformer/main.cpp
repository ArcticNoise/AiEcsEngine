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

using namespace x2d;
using namespace platformer;

int main() {
    Application app;
    auto &world = app.GetWorld();

    Signature windowSig{};
    app.RegisterSystem<WindowSystem>(ESystemPhase::eSystemPhase_Init, windowSig, 640, 480,
                                     std::string("Platformer"));

    Signature timeSig{};
    app.RegisterSystem<TimeSystem>(ESystemPhase::eSystemPhase_Init, timeSig);

    Signature inputSig;
    inputSig.set(ComponentTypeId<InputStateComponent>());
    app.RegisterSystem<InputSystem>(ESystemPhase::eSystemPhase_Update, inputSig);

    Signature moveSig;
    moveSig.set(ComponentTypeId<TransformComponent>());
    moveSig.set(ComponentTypeId<InputStateComponent>());
    moveSig.set(ComponentTypeId<PlayerComponent>());
    app.RegisterSystem<MovementSystem>(ESystemPhase::eSystemPhase_Update, moveSig);

    Signature camSig;
    camSig.set(ComponentTypeId<TransformComponent>());
    camSig.set(ComponentTypeId<CameraFollowComponent>());
    auto &camSys =
        app.RegisterSystem<CameraFollowSystem>(ESystemPhase::eSystemPhase_Update, camSig);

    Signature mapSig;
    mapSig.set(ComponentTypeId<TransformComponent>());
    mapSig.set(ComponentTypeId<TileMapComponent>());
    app.RegisterSystem<TileMapSystem>(ESystemPhase::eSystemPhase_Render, mapSig, camSys);

    Entity player = world.CreateEntity();
    world.AddComponent<TransformComponent>(player, {32.0f, 32.0f, 0.0f, 1.0f});
    world.AddComponent<InputStateComponent>(player, {});
    world.AddComponent<PlayerComponent>(player, {});
    world.AddComponent<CameraFollowComponent>(player, {0.0f, 0.0f});

    static const int mapWidth = 10;
    static const int mapHeight = 8;
    static const int mapData[mapWidth * mapHeight] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    Entity map = world.CreateEntity();
    world.AddComponent<TransformComponent>(map, {0.0f, 0.0f, 0.0f, 1.0f});
    world.AddComponent<TileMapComponent>(map, {mapData, mapWidth, mapHeight, 32});

    app.ChangeState(EAppState::eAppState_MainMenu);

    int frame = 0;
    while (!WindowShouldClose() && frame < 180) {
        if (frame == 60) {
            app.ChangeState(EAppState::eAppState_Pause);
        }
        if (frame == 120) {
            app.ChangeState(EAppState::eAppState_Shutdown);
        }
        app.Update();
        ++frame;
    }

    return 0;
}
