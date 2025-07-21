add_executable(
    platformer
    main.cpp
    Components.hpp
    CameraFollowSystem.cpp
    CameraFollowSystem.hpp
    MovementSystem.cpp
    MovementSystem.hpp
    TileMapSystem.cpp
    TileMapSystem.hpp
)

target_link_libraries(platformer PRIVATE x2d)
target_include_directories(platformer PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_definitions(platformer PRIVATE X2D_PLATFORM_${X2D_PLATFORM_UPPER})
x2d_enable_strict_warnings(platformer)
