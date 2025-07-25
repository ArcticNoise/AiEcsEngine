# X2D Engine Documentation

This guide covers the basic setup and usage of **X2D** – a cross-platform 2D game engine written in C++20. Follow the steps below to create and build a new project using the engine.

## Prerequisites

- C++20 compatible compiler (GCC 14, Clang 16, or MSVC 19.38)
- CMake 3.28.3
- Raylib dependencies (on Linux: `libxrandr-dev`, `libxinerama-dev`, `libxcursor-dev`, `libxi-dev`)

## Building the Engine

1. Clone the repository and initialise submodules:
   ```bash
   git clone <repo-url>
   cd AiEcsEngine
   ```
2. Install toolchains (Linux example):
   ```bash
   ./scripts/install_toolchains.sh
   ```
3. Configure and build:
   ```bash
   cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
   cmake --build build --config Release
   ```
4. Run tests to verify the build:
   ```bash
   ctest --test-dir build --output-on-failure
   ```

## Creating a New Project

1. Add `AiEcsEngine` as a subdirectory in your own CMake project:
   ```cmake
   add_subdirectory(AiEcsEngine)
   target_link_libraries(your_game PRIVATE x2d)
   ```
2. Include the engine headers in your code:
   ```cpp
   #include "Application/Application.hpp"
   #include "ECS/TransformComponent.hpp"
   ```
3. Create an `x2d::Application` instance and register systems:
   ```cpp
   x2d::Application app;
   x2d::World& world = app.GetWorld();

   x2d::Signature windowSig{};
   app.RegisterSystem<x2d::WindowSystem>(x2d::ESystemPhase::eSystemPhase_Init,
                                         windowSig, 800, 600, "My Game");
   // Register more systems here
   ```
4. Create entities and attach components:
   ```cpp
   x2d::Entity player = world.CreateEntity();
   world.AddComponent<x2d::TransformComponent>(player, {32.f, 32.f, 0.f, 1.f});
   ```
5. Start the main loop by calling `app.Update()` each frame:
   ```cpp
   while (!WindowShouldClose()) {
       app.Update();
   }
   ```

## Creating and Registering Components

To define a custom component, declare a plain struct aligned to 16 bytes and keep
its size below 64 bytes.

```cpp
// HealthComponent.hpp
#pragma once

namespace demo
{

struct alignas(16) HealthComponent
{
    int hp;
};
static_assert(sizeof(HealthComponent) <= 64);

} // namespace demo
```

Add the component to an entity to register its storage automatically:

```cpp
x2d::Entity e = world.CreateEntity();
world.AddComponent<demo::HealthComponent>(e, {100});
```

## Creating and Registering Systems

A system derives from `x2d::ISystem`. Register it with the application using a
signature describing the components it requires.

```cpp
class DamageSystem final : public x2d::ISystem
{
public:
    void Update(const x2d::View& view) override
    {
        if (m_World == nullptr)
        {
            return;
        }
        for (std::size_t i = 0; i < view.count; ++i)
        {
            const x2d::Entity e = view.entities[i];
            auto& health = m_World->GetComponent<demo::HealthComponent>(e);
            health.hp -= 1;
        }
    }
};
```

Register the system and assign its signature:

```cpp
x2d::Signature damageSig{};
damageSig.set(x2d::ComponentTypeId<demo::HealthComponent>());
app.RegisterSystem<DamageSystem>(x2d::ESystemPhase::eSystemPhase_Update,
                                 damageSig);
```

## Example

The `examples/platformer` directory demonstrates a small game built with X2D. It registers input, camera, and map systems and spawns a simple player entity. Use it as a reference for your own projects.

## Additional Resources

- [Engine Source](../engine)
- [Example Projects](../examples)
- [Unit Tests](../tests)

