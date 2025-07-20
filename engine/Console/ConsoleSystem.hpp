#pragma once

#include <raylib.h>

#include "Console/ConsoleComponent.hpp"
#include "ECS/System.hpp"
#include "Profiling/Profiler.hpp"

namespace x2d {

class ConsoleSystem final : public ISystem {
  public:
    void Update(const View &view) override;
};

} // namespace x2d
