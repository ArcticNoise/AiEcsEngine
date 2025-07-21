cmake_minimum_required(VERSION 3.28.3)
project(X2D LANGUAGES CXX VERSION 1.0.0)

set(SUPPORTED_PLATFORMS DESKTOP WEB PS XBOX SWITCH)
set(X2D_PLATFORM "DESKTOP" CACHE STRING "Target platform")
set_property(CACHE X2D_PLATFORM PROPERTY STRINGS ${SUPPORTED_PLATFORMS})
list(FIND SUPPORTED_PLATFORMS ${X2D_PLATFORM} idx)
if(idx EQUAL -1)
    message(FATAL_ERROR "Invalid X2D_PLATFORM: ${X2D_PLATFORM}")
endif()
string(TOUPPER "${X2D_PLATFORM}" X2D_PLATFORM_UPPER)

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
if(X2D_PLATFORM_UPPER STREQUAL "WEB")
    set(PLATFORM_WEB ON)
    set(PLATFORM_DESKTOP OFF)
else()
    set(PLATFORM_DESKTOP ON)
endif()
FetchContent_MakeAvailable(raylib)

add_library(x2d STATIC)
file(GLOB_RECURSE X2D_SRC CONFIGURE_DEPENDS engine/*.cpp)
foreach(p ${SUPPORTED_PLATFORMS})
    if(NOT X2D_PLATFORM_UPPER STREQUAL p)
        list(FILTER X2D_SRC EXCLUDE REGEX "engine/Platform/${p}/.*\\.cpp")
    endif()
endforeach()
target_sources(x2d PRIVATE ${X2D_SRC})

target_link_libraries(x2d PUBLIC raylib)

target_include_directories(x2d PUBLIC engine)

target_compile_definitions(x2d PUBLIC X2D_PLATFORM_${X2D_PLATFORM_UPPER})

x2d_enable_strict_warnings(x2d)

enable_testing()

FetchContent_Declare(catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.3)
FetchContent_MakeAvailable(catch2)

file(GLOB TEST_SRC CONFIGURE_DEPENDS tests/*Test.cpp)
add_executable(x2d_tests ${TEST_SRC})
target_link_libraries(x2d_tests PRIVATE Catch2::Catch2WithMain x2d)
x2d_enable_strict_warnings(x2d_tests)
target_compile_definitions(x2d_tests PRIVATE X2D_PLATFORM_${X2D_PLATFORM_UPPER})
add_test(NAME x2d_tests COMMAND x2d_tests)
