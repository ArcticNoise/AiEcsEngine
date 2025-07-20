#include <catch2/catch_test_macros.hpp>

#include "Application/Application.hpp"
#include "ECS/SystemManager.hpp"

using namespace x2d;

namespace
{
struct PhaseLogSystem : ISystem
{
    std::vector<int>& log;
    int value;
    PhaseLogSystem(std::vector<int>& l, int v) : log(l), value(v) {}
    void Update(const View&) override { log.push_back(value); }
};
} // namespace

TEST_CASE("System phases run in order", "[phase]")
{
    std::vector<int> log;
    Application app;
    app.RegisterSystem<PhaseLogSystem>(ESystemPhase::eSystemPhase_Update, {}, log, 1);
    app.RegisterSystem<PhaseLogSystem>(ESystemPhase::eSystemPhase_Render, {}, log, 2);

    app.Update();

    const std::vector<int> expected{1,2};
    REQUIRE(log == expected);
}
