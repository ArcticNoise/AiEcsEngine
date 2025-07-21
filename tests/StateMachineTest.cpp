#include <catch2/catch_test_macros.hpp>

#include "Application/Application.hpp"
#include "ECS/SystemManager.hpp"

namespace
{

struct TrackingState : x2d::IState
{
    std::string name;
    std::vector<std::string>& log;
    int enterCount{0};
    int exitCount{0};
    int tickCount{0};

    TrackingState(std::string n, std::vector<std::string>& l)
        : name(std::move(n)), log(l) {}

    void Enter() override
    {
        ++enterCount;
        log.push_back("enter " + name);
    }

    void Exit() override
    {
        ++exitCount;
        log.push_back("exit " + name);
    }

    void Tick() override
    {
        ++tickCount;
        log.push_back("tick " + name);
    }
};

struct LogSystem : x2d::ISystem
{
    std::vector<std::string>& log;
    explicit LogSystem(std::vector<std::string>& l) : log(l) {}
    void Update(const x2d::View&) override { log.push_back("update"); }
};

} // namespace

TEST_CASE("StateMachine transitions", "[fsm]")
{
    std::vector<std::string> log;
    x2d::StateMachine sm;

    sm.PushState(std::make_unique<TrackingState>("Boot", log));
    sm.PushState(std::make_unique<TrackingState>("Menu", log));
    sm.PopState();
    sm.ReplaceState(std::make_unique<TrackingState>("Gameplay", log));
    sm.ReplaceState(std::make_unique<TrackingState>("Pause", log));

    const std::vector<std::string> expected{
        "enter Boot",
        "enter Menu",
        "exit Menu",
        "exit Boot",
        "enter Gameplay",
        "exit Gameplay",
        "enter Pause"};
    REQUIRE(log == expected);
}

TEST_CASE("Application update order", "[fsm]")
{
    std::vector<std::string> log;
    x2d::Application app;
    auto& sm = app.GetStateMachine();
    sm.PushState(std::make_unique<TrackingState>("A", log));
    app.RegisterSystem<LogSystem>(x2d::ESystemPhase::eSystemPhase_Update, {}, log);

    app.Update();

    const std::vector<std::string> expected{"enter A", "tick A", "update"};
    REQUIRE(log == expected);
}

