#include "Profiling/Profiler.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>
#include <filesystem>

TEST_CASE("Profiler writes csv") {
    const char *path = "profile.csv";
    x2d::Profiler::SetOutput(path);
    x2d::Profiler::SetEnabled(true);
    {
        X2D_PROFILE_SCOPE("test");
    }
    x2d::Profiler::SetEnabled(false);
    x2d::Profiler::Flush();
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
