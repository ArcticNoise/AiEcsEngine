#include <catch2/catch_test_macros.hpp>

#include "Application/Application.hpp"
#include "ECS/SystemManager.hpp"
namespace
{
struct PhaseLogSystem : x2d::ISystem
{
    std::vector<int>& log;
    int value;
    PhaseLogSystem(std::vector<int>& l, int v) : log(l), value(v) {}
    void Update(const x2d::View&) override { log.push_back(value); }
};
} // namespace

TEST_CASE("System phases run in order", "[phase]")
{
    std::vector<int> log;
    x2d::Application app;
    app.RegisterSystem<PhaseLogSystem>(x2d::ESystemPhase::eSystemPhase_Update, {}, log, 1);
    app.RegisterSystem<PhaseLogSystem>(x2d::ESystemPhase::eSystemPhase_Render, {}, log, 2);

    app.Update();

    const std::vector<int> expected{1,2};
    REQUIRE(log == expected);
}
