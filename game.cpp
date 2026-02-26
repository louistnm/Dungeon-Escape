//
// Created by Louis Tennier on 1/28/26.
//

#include "game.h"

#include "input.h"

Game::Game(std::string title, int width, int height)
    : graphics{title, width, height}, world{31,11}, camera{graphics, 64}, dt{1.0/60.0}, lag{0.0},
performance_frequency{SDL_GetPerformanceFrequency()},
prev_counter{SDL_GetPerformanceCounter()} { //constructing private data in initializer list

    //load the first "level"
    //boundary walls
    world.add_platform(0,0,30,1);
    world.add_platform(0,0,1,10);
    world.add_platform(30,0,1,10);
    world.add_platform(0,10,31,1);

    //platforms
    world.add_platform(3,7,4,1);
    world.add_platform(13,4,6,1);

    player = world.create_player();
    camera.set_location(player->physics.position);
}

void Game::input() {
    player->input->get_input();
    camera.handle_input();
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (float)performance_frequency; //casting C style cause SDL is C
    prev_counter = now;
    while (lag >= dt) {
        player->update(world, dt); //player update before world
        world.update(dt);
        //put the camera slightly ahead of the player
        float L = length(player->physics.velocity);
        Vec displacement = 8.0f * player->physics.velocity / (1.0f + L);
        camera.update(player->physics.position + displacement, dt);
        lag -= dt; //accumulate lag enough so that you update world every 60th of a second
    }
}

void Game::render() {
    //clear
    graphics.clear();

    //draw world
    camera.render(world.tilemap);

    //draw the player
    auto [player_position, color] = player->get_sprite();
    camera.render(player_position, color);

    //update()
    graphics.update();
}

