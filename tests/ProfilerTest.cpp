#include "Profiling/Profiler.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>
#include <filesystem>

using namespace x2d;

TEST_CASE("Profiler writes csv") {
    const char *path = "profile.csv";
    Profiler::SetOutput(path);
    Profiler::SetEnabled(true);
    {
        X2D_PROFILE_SCOPE("test");
    }
    Profiler::SetEnabled(false);
    Profiler::Flush();
    FILE *f = nullptr;
#ifdef _MSC_VER
    fopen_s(&f, path, "r");
#else
    f = std::fopen(path, "r");
#endif
    REQUIRE(f != nullptr);
    char buffer[64];
    REQUIRE(std::fgets(buffer, sizeof(buffer), f) != nullptr); // header
    REQUIRE(std::fgets(buffer, sizeof(buffer), f) != nullptr); // entry
    std::fclose(f);
    std::filesystem::remove(path);
}
