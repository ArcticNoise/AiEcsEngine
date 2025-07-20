#pragma once

#include <cstdint>

namespace x2d
{

using Entity = std::uint32_t;

inline constexpr std::uint32_t s_IndexBits = 22U;
inline constexpr std::uint32_t s_GenerationBits = 10U;
inline constexpr std::uint32_t s_IndexMask = (1U << s_IndexBits) - 1U;
inline constexpr std::uint32_t s_GenerationMask = (1U << s_GenerationBits) - 1U;

inline constexpr Entity MakeEntity(std::uint32_t index, std::uint32_t generation)
{
    return (generation << s_IndexBits) | index;
}

inline constexpr std::uint32_t GetIndex(Entity entity)
{
    return entity & s_IndexMask;
}

inline constexpr std::uint32_t GetGeneration(Entity entity)
{
    return (entity >> s_IndexBits) & s_GenerationMask;
}

} // namespace x2d

