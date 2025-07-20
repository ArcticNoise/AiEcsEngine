#pragma once

#include "ECS/System.hpp"

namespace x2d
{

class TimeSystem final : public ISystem
{
public:
    explicit TimeSystem(float fixedDelta = 1.0f / 60.0f);

    void Update(const View& view) override;

    [[nodiscard]] float GetDeltaTime() const;
    [[nodiscard]] float GetFixedDelta() const;
    [[nodiscard]] bool ShouldRunFixedUpdate() const;
    void AdvanceFixedUpdate();

private:
    float m_FixedDelta;
    float m_DeltaTime{0.0f};
    float m_Accumulator{0.0f};
};

} // namespace x2d
