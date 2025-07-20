#include "TimeSystem.hpp"

#include <raylib.h>

namespace x2d
{

TimeSystem::TimeSystem(float fixedDelta)
    : m_FixedDelta(fixedDelta)
{
}

void TimeSystem::Update(const View&)
{
    m_DeltaTime = GetFrameTime();
    m_Accumulator += m_DeltaTime;
}

float TimeSystem::GetDeltaTime() const
{
    return m_DeltaTime;
}

float TimeSystem::GetFixedDelta() const
{
    return m_FixedDelta;
}

bool TimeSystem::ShouldRunFixedUpdate() const
{
    return m_Accumulator >= m_FixedDelta;
}

void TimeSystem::AdvanceFixedUpdate()
{
    m_Accumulator -= m_FixedDelta;
}

} // namespace x2d
