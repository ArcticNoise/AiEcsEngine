cmake_minimum_required(VERSION 3.29)
project(X2D LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/StrictFlags.cmake)
include(FetchContent)

# Raylib
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5)
set(BUILD_SHARED_LIBS OFF)
set(PLATFORM_DESKTOP ON)
FetchContent_MakeAvailable(raylib)

add_library(x2d STATIC)
file(GLOB_RECURSE X2D_SRC CONFIGURE_DEPENDS engine/*.cpp)
target_sources(x2d PRIVATE ${X2D_SRC})

target_link_libraries(x2d PUBLIC raylib)

target_include_directories(x2d PUBLIC engine)

x2d_enable_strict_warnings(x2d)

enable_testing()
