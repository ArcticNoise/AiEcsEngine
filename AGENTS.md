# AGENTS.md

## Purpose

Authoritative, non‑negotiable instructions for building **X2D** – a cross‑platform 2D game engine written in modern C++20, using Raylib for platform/graphics I/O, a data‑oriented ECS core, and CMake/FetchContent for builds. Codex/OpenAI must follow this document verbatim; no deviation, no omissions.

---

## Global Principles

1. **Engine ≠ Game**: X2D is a standalone static/shared library consumed by game projects.
2. **Data‑Oriented ECS**: Entities are IDs, components are POD data in SoA storages, systems are logic operating on dense views.
3. **Platform Isolation**: All OS‑specific code resides in `engine/Platform/` behind minimal interfaces.
4. **Strict Coding Standard**: See §Coding Rules. Violations are compile‑time errors.
5. **CI Discipline**: All targets must compile and unit tests must pass on Windows, Linux, macOS.

---

## Project Layout

```
engine/            ← X2D source
  ECS/             ← core ECS (entity, component, system, world)
  Graphics/        ← RenderSystem2D, texture pipeline
  Physics/         ← basic AABB physics
  Platform/        ← OS abstraction + Raylib glue
examples/          ← demo games
extern/            ← third‑party libs via FetchContent
tests/             ← Catch2 unit tests (postfix *Test.cpp)
cmake/             ← helper scripts/toolchains
CMakeLists.txt
```

---

## Build System

### Root `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.28.3)
project(X2D LANGUAGES CXX VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/StrictFlags.cmake) # enables -Wall -Wextra -Werror etc.
include(FetchContent)

# Raylib
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 5.5)
set(BUILD_SHARED_LIBS OFF)
set(PLATFORM_DESKTOP ON)
FetchContent_MakeAvailable(raylib)

add_library(x2d STATIC)
file(GLOB_RECURSE X2D_SRC CONFIGURE_DEPENDS engine/*.cpp)
target_sources(x2d PRIVATE ${X2D_SRC})

target_link_libraries(x2d PUBLIC raylib)

# headers
target_include_directories(x2d PUBLIC engine)
```

### CI Matrix

```
windows-latest  msvc  v143
ubuntu-latest   clang 16
macos-14        clang 16
```

Each job: `cmake -B build -S . -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release && ctest --output-on-failure`.

---

## Development Roadmap

The roadmap enumerates **mandatory milestones**. Do not skip, reshuffle, or merge stages without updating this document.

### Stage 0 – Environment

- Install toolchains (MSVC 19.38, GCC 14, Clang 16).
- Configure Git hooks: `pre-commit` runs **clang‑format** + **clang‑tidy**.
- Initialise GitHub Actions CI (matrix defined in §Build System).

### Stage 1 – Skeleton & CMake

- Create directory layout (§Project Layout).
- Add root `CMakeLists.txt` and strict compiler flags.
- Integrate **Raylib** via FetchContent.
- Add empty `x2d` target; ensure it links Raylib and compiles on all CI targets.

### Stage 2 – Core ECS Foundation

The ECS layer must strictly follow **data‑oriented best practices**.

| Sub‑module               | Key Requirements                                                                                                                                                                                 | Best‑Practice Rationale                                                            |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------- |
| **EntityManager**        | 32‑bit `Entity` = **index (22 bits)** + **generation (10 bits)**.Free‑list for O(1) create/destroy.Validate handles via generation match.                                                        | Handle recycling without overflow; prevents dangling IDs.                          |
| **ComponentStorage**     | Template `ComponentStorage<TComponent>`.Dense `std::vector<TComponent>` (SoA) + sparse set for O(1) lookup.`Add` = push‑back dense + patch sparse.`Remove` = swap‑and‑pop to keep dense packing. | Maximises cache locality, avoids holes; swap removal keeps index validity cheap.   |
| **Signature**            | `std::bitset<64>`; each component type receives unique bit via `constexpr ComponentTypeID<T>()`.Update entity signature on add/remove; use bitwise AND to match systems.                         | Branch‑free filtering; 64 bits == no dynamic alloc; fits in single CPU register.   |
| **SystemManager**        | `RegisterSystem<S>()` captures required signature.World builds per‑system **view** once per frame.Views are arrays of entity indices (pointer + count) to iterate linearly.                      | Building view once amortises filtering cost; iteration pointer‑chasing eliminated. |
| **World**                | Thin façade: forwards to subsystems; **no business logic**.Exposes templates:`template<class T> void AddComponent(Entity, T&&);``template<class... Ts> View<Ts...> GetView();`                   | Keeps public API minimal; games remain decoupled from internals.                   |
| **Archetype Cache**      | Optional: chunk entities by identical signatures (archetype). Each chunk = 16 KB SoA arrays for each component in archetype.                                                                     | Further reduces random memory access for heavy entity counts.                      |
| **Component Naming**     | **Every component type ends with **``.Examples: `TransformComponent`, `VelocityComponent`, `SpriteComponent`.                                                                                    | Immediate visual recognition; enforces uniformity across codebase.                 |
| **One‑Frame Components** | Marker template `OneFrame<TComponent>`.Storage auto‑clears dense array at end of frame.Use for events (e.g., `DamageEventComponent`).                                                            | Eliminates manual cleanup; prevents stale events.                                  |
| **Unit Tests**           | Catch2: create/destroy 1 M entities < 50 ms.Verify signatures, component swap‑erase integrity, OneFrame clear.                                                                                   | Guarantees performance budget and correctness.                                     |

Additional directives:

- **Alignment**: ensure each component struct uses `alignas(16)` and fits ≤ 64 bytes to stay within a single cache line.
- **Constructor Policy**: components must be POD; initialisation handled by caller; no default expensive constructors.
- **Thread Safety**: World lock‑free read during Update; writes (add/remove) deferred via command buffer, applied post‑frame.

> **Rule**: When adding a new component, include the word `` at the end of the type name and decide whether it is **persistent** or **OneFrame**. Document rationale in code‑comment.\
> *Violation blocks CI.*

### Stage 3 – Game **State Machine** Layer – Game **State Machine** Layer

Implement a lightweight **Finite State Machine (FSM)** controlling high‑level application flow.

| State      | Purpose                      | Enter / Exit Actions             |
| ---------- | ---------------------------- | -------------------------------- |
| `Boot`     | Minimal init, splash screen. | Load config, verify GPU.         |
| `Preload`  | Async asset streaming.       | Spawn progress bar entity.       |
| `MainMenu` | Title screen, settings.      | Register UI systems, play music. |
| `Gameplay` | Active game loop.            | Enable gameplay systems.         |
| `Pause`    | Overlay, freeze simulation.  | Disable physics tick.            |
| `Shutdown` | Graceful teardown.           | Flush save data, close window.   |

Implementation details:

- `engine/Application/state_machine.*` – template‑driven FSM with **push/pop/replace**.
- States own their **World** instance or manipulate a shared one via scoped layers.
- Expose `void ChangeState(EAppState next);` to games.
- Hook FSM tick **before** ECS `Update` each frame; render last.
- Unit‑test: correct transition order, no double entry/exit.

### Stage 4 – Platform & Rendering

- **WindowSystem** – wraps `InitWindow`, `CloseWindow`, dispatches resize events to ECS.
- **RenderSystem2D** – iterates `Transform + Sprite`; batch draw by texture to reduce state changes.
- **InputSystem** – maps Raylib inputs to `InputState` components.
- **TimeSystem** – centralises `deltaTime`; provides fixed‑step scheduler for physics.
- **System Phases** – introduce **Init**, **FixedUpdate**, **Update**, **LateUpdate**, **Render** pipelines ordered per‑frame. World triggers systems in phase order; register with enum tag.

### Stage 5 – Physics & Events

- Add `ColliderAABB`, `RigidBody2D` components.
- `PhysicsSystem` – fixed‑step integration, broad‑phase sweep, narrow‑phase AABB.
- **Event Bus** – lock‑free queues (`EventQueue<T>`); post collision/trigger events.

### Stage 6 – Utilities & Tooling

- **AssetLoader** – thread‑pool async load; cache by path hash.
- **Profiler** – RAII scope timer macro, dumps CSV; integrate Tracy optionally.
- **Console** – in‑game developer console entity allowing ECS commands (`spawn`, `profile on`).

### Stage 7 – Cross‑Platform Hooks

- `engine/Platform/` – separate implementations per OS; compile‑time switch via CMake option `X2D_PLATFORM`.
- Provide stubs for consoles – compile but no‑op without SDK.
- **Web Build** – add `PLATFORM_WEB` target using Emscripten.

### Stage 8 – Example Game `examples/platformer`

- Demonstrates custom components/systems, FSM transitions (MainMenu → Gameplay → Pause → Shutdown), camera follow, tiled map.
- Build script links against **x2d** only.

### Stage 9 – Optimisation Cycle

- Profile on low‑end GPU (Intel HD 620) with 1 000 sprites.
- Eliminate per‑frame allocations; verify **cache miss < 3 %** in hot loops via Callgrind.
- Parallelise independent systems using simple job system (std::jthread + work‑stealing queue).

### Stage 10 – Release 1.0

- Tag `v1.0.0`.
- Generate changelog, upload binary artifacts for each platform.
- Freeze API; subsequent changes bump **MAJOR** per SemVer.

---

## Coding Rules (MANDATORY)

### Language & Build

- C++20 only. No compiler extensions.
- Use `#pragma once` in every header.
- Use forward declarations wherever possible.
- **Never** nest public classes inside other classes.
- **Never** write `using namespace`. Always qualify (`std::vector`, `raylib::Texture`, etc.). Unity builds rely on this.
- Hide module‑local symbols inside anonymous namespaces.

### Naming Conventions

| Element                  | Format                                                                                                | Prefix/Suffix                                                     |
| ------------------------ | ----------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- |
| **Classes/Structs**      | UpperCamelCase                                                                                        | *No* `C`, `T`, `S` prefixes (template parameters start with `T`). |
| **Components**           | UpperCamelCase **ending with **``.`TransformComponent`, `DamageEventComponent`.                       | Distinguish data objects at a glance.                             |
| **One‑Frame Components** | Same as Components; annotate with `OneFrame<...>` wrapper at registration.                            | Signals automatic disposal after frame.                           |
| **Enums (declaration)**  | Prefix `E`, UpperCamelCase.`enum class EColor { … };`                                                 |                                                                   |
| **Enums (values)**       | Prefix `e`, UpperCamelCase, optional type prefix: `eColor_Red`                                        |                                                                   |
| **Typedefs/using**       | UpperCamelCase, template form prefer `using` with `T`: `using TStrVec = std::vector<std::string>;`    |                                                                   |
| **Members**              | Prefix `m_` + UpperCamelCase: `int32 m_Position;`                                                     |                                                                   |
| **Staticals/constexpr**  | Prefix `s_` + UpperCamelCase.                                                                         |                                                                   |
| **Globals**              | Discouraged; if needed prefix `g_` + UpperCamelCase.                                                  |                                                                   |
| **Function/Method**      | UpperCamelCase, no underscores: `UpdatePosition`. Always mark `override` or `final` where applicable. |                                                                   |
| **Arguments**            | LowerCamelCase. Use `inout_`, `out_` prefixes for refs: `void Foo(int& out_Count);`                   |                                                                   |
| **Locals**               | LowerCamelCase: `frameTime`.                                                                          |                                                                   |

### Pointers & Const & Const

- Pointer/reference modifiers immediately follow type: `MyType* ptr`, `MyType& ref`.
- Separate `const` with a space: `void Func() const`.
- Prefer `const` for inputs; use `const&` for complex types.
- Do **not** return `const` values, prefer `const&` if needed.
- Temporary locals should be `const`.

### Virtual, Override, Final

- Mark classes `final` if inheritance not intended.
- Use `override`/`final` on every overriding method.
- Apply `virtual` **only** where polymorphism is required.

### Initialization & Bracing

- Initialise all members in constructor initialiser list:
  ```cpp
  Foo() : m_Ptr(nullptr), m_Pos{0,0,0} {}
  ```
- Avoid Egyptian braces; use Allman style.

### Casting

- Never use C‑style casts.
- Use `ls::checked_cast<>()` for class types and `ls::checked_numcast<>()` for numeric casts.
- Prefer dedicated functions over casts (e.g., `strlen` vs creating `std::string`).

### Macros

- ALL\_CAPS\_WITH\_UNDERSCORES.
- Avoid macros for constants; prefer `constexpr` in anonymous namespace.

### Miscellaneous

- Use `nullptr`, not `NULL`.
- Use range‑based `for` when possible.
- Do not cast away `const` unless interfacing with const‑incorrect API.

---

## Testing Policy

- Every public API and critical path must have a Catch2 unit test (file suffix `*Test.cpp`).
- Tests must fail on logic errors; CI blocks merge on red.
- If a legacy area lacks tests, the first developer touching it must add them.

---

## Commit Discipline

- No merge to `main` without green CI **unless the end‑user explicitly instructs to merge despite failures**.
- Squash merge; commit messages follow Conventional Commits (`feat:`, `fix:` …).

---

End of AGENTS.md

