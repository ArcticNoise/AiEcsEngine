#include "ConsoleSystem.hpp"

#include "ECS/World.hpp"
#include <cstring>

namespace x2d {

void ConsoleSystem::Update(const View &view) {
    if (m_World == nullptr) {
        return;
    }

    for (std::size_t i = 0; i < view.count; ++i) {
        const Entity e = view.entities[i];
        auto &console = m_World->GetComponent<ConsoleComponent>(e);
        if (IsKeyPressed(KEY_GRAVE)) {
            console.open = !console.open;
        }
        if (!console.open) {
            continue;
        }
        int ch = GetCharPressed();
        while (ch > 0 && console.length < sizeof(console.input) - 1) {
            console.input[console.length++] = static_cast<char>(ch);
            ch = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && console.length > 0) {
            --console.length;
            console.input[console.length] = '\0';
        }
        if (IsKeyPressed(KEY_ENTER)) {
            console.input[console.length] = '\0';
            if (std::strcmp(console.input, "spawn") == 0) {
                static_cast<void>(m_World->CreateEntity());
            } else if (std::strcmp(console.input, "profile on") == 0) {
                Profiler::SetEnabled(true);
            } else if (std::strcmp(console.input, "profile off") == 0) {
                Profiler::SetEnabled(false);
            }
            console.length = 0;
            console.input[0] = '\0';
        }

        if (console.open) {
            DrawRectangle(0, 0, GetScreenWidth(), 20, DARKGRAY);
            console.input[console.length] = '\0';
            DrawText(console.input, 5, 2, 16, RAYWHITE);
        }
    }
}

} // namespace x2d
