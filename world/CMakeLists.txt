project(world)

add_library(world
        world.h world.cpp
        tilemap.cpp tilemap.h
        camera.cpp camera.h
        game_object.cpp game_object.h
        action.cpp action.h
        fsm.cpp fsm.h
        states.h states.cpp
        input.cpp input.h
        keyboard_input.h keyboard_input.cpp
        audio.cpp audio.h
        level.cpp level.h
        events.h events.cpp
        quadtree.h quadtree.cpp
        projectile.h projectile.cpp
        ai_input.h ai_input.cpp
)

target_include_directories(world PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(world PUBLIC SDL3::SDL3 SDL3_mixer::SDL3_mixer utility graphics)