#pragma once

#include <cstddef>

namespace x2d {

struct ConsoleComponent {
    bool open;
    char input[256];
    std::size_t length;
};

} // namespace x2d
