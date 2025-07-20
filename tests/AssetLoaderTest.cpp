#include "Assets/AssetLoader.hpp"
#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <thread>

using namespace x2d;

TEST_CASE("AssetLoader caches by path") {
    AssetLoader loader(1);
    auto future1 = loader.Load<int>("foo", [](const std::string &) { return 42; });
    auto future2 = loader.Load<int>("foo", [](const std::string &) { return 43; });
    REQUIRE(future1.get() == future2.get());
}
