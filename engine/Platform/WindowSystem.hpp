#pragma once

#include <string>
#include <raylib.h>

#include "ECS/System.hpp"
#include "ECS/OneFrame.hpp"
#include "Platform/WindowResizeEventComponent.hpp"

namespace x2d
{

class WindowSystem final : public ISystem
{
public:
    WindowSystem(int width, int height, std::string title);
    ~WindowSystem() override;

    void Update(const View& view) override;

private:
    bool m_Initialized{false};
};

} // namespace x2d
