#pragma once

#include <bitset>
#include <cstddef>

namespace x2d
{

using Signature = std::bitset<64>;

inline std::size_t NextComponentTypeId()
{
    static std::size_t s_Next = 0;
    return s_Next++;
}

template<class T>
[[nodiscard]] inline std::size_t ComponentTypeId()
{
    static const std::size_t id = NextComponentTypeId();
    return id;
}

} // namespace x2d

