//
// Created by Louis Tennier on 1/28/26.
//

#include "game.h"

#include <iostream>

#include "ai_input.h"
#include "asset_manager.h"
#include "fsm.h"
#include "input.h"
#include "game_object.h"
#include "events.h"
#include "keyboard_input.h"
#include "states.h"

Game::Game(std::string title, int width, int height)
:graphics{title, width, height}, camera{graphics, 64}, dt{1.0 / 60.0}, performance_frequency{SDL_GetPerformanceFrequency()},
prev_counter{SDL_GetPerformanceCounter()}, lag{0.0} {

    //load events
    get_events();

    // create the world for the first level

    // Give player its assets then put it in the correct state
    create_player();
    AssetManager::get_game_object_details("player", graphics, *player);

    //load first level
    load_level();
}

Game::~Game() {
    delete world;
    for (auto [_, event]: events) {
        delete event;
    }
}

void Game::handle_event(SDL_Event* event) {
    switch (mode)
    {
    case GameMode::Playing:
        player->input->collect_discrete_event(event);
        break;
    }
    player->input->collect_discrete_event(event);
}

void Game::input() {
    switch (mode)
    {
    case GameMode::Playing:
        player->input->get_input();
        camera.handle_input();
    }
}

void Game::update() {
    Uint64 now = SDL_GetPerformanceCounter();
    lag += (now - prev_counter) / (float)performance_frequency; //casting C style cause SDL is C
    prev_counter = now;
    while (lag >= dt) {
        switch (mode)
        {
        case GameMode::Playing:
            for (auto obj : world->game_objects) {
                obj->input->handle_input(*world, *obj);
            }

            world->update(dt);
            //put the camera slightly ahead of the player
            float L = length(player->physics.velocity);
            Vec displacement = 8.0f * player->physics.velocity / (1.0f + L);
            camera.update(player->physics.position + displacement, dt);

            //check for level end
            if (world->end_level) {
                load_level();
            }

            //check for game over
            if (world -> end_game) {
                mode = GameMode::GameOver;
            }
            break;
        }
        lag -= dt; //accumulate lag enough so that you update world every 60th of a second
    }
}

void Game::render() {
    //clear
    graphics.clear();

    //draw world
    camera.render(world->tilemap);

    //draw the player

    camera.render(*player);

    //enemies
    for (auto& obj : world->game_objects) {
        camera.render(*obj);
    }

    if (mode == GameMode::GameOver) {
        camera.render_game_over();
    }

    //update()
    graphics.update();
}

void Game::get_events() {
    events["next_level"] = new NextLevel();
}

void Game::load_level() {
    std::string level_name = "level_" + std::to_string(++current_level);
    Level level{level_name};
    AssetManager::get_level_details(graphics, level);

    //create the world
    delete world;
    world = new World(level, audio, player.get(), events);

    //assets for objects
    for (auto& obj : world->game_objects) {
        if (obj == world->player) continue;
        AssetManager::get_game_object_details(obj->obj_name + "-enemy", graphics, *obj, true);
    }

    player->physics.position = {static_cast<float>(level.player_spawn_location.x), static_cast<float>(level.player_spawn_location.y)};
    player->fsm->current_state->on_enter(*world, *player);
    camera.set_location(player->physics.position);
    audio.play_sounds("background", true);
}

void Game::create_player() {
    // Create FSM
    Transitions transitions = {
        {{StateType::Standing, Transition::Jump}, StateType::InAir}, //if standing and jump, go to inair
        {{StateType::InAir, Transition::Stop}, StateType::Standing},
        {{StateType::Standing, Transition::Move}, StateType::Running},
        {{StateType::Standing, Transition::Sprint}, StateType::Sprinting},
        {{StateType::Standing, Transition::Fall}, StateType::Falling},
        {{StateType::Standing, Transition::Roll}, StateType::Rolling},
        {{StateType::Running, Transition::Stop}, StateType::Standing},
        {{StateType::Running, Transition::Sprint}, StateType::Sprinting},
        {{StateType::Running, Transition::Jump}, StateType::InAir},
        {{StateType::Running, Transition::Roll}, StateType::Rolling},
        {{StateType::Running, Transition::Fall}, StateType::Falling},
        {{StateType::Sprinting, Transition::Stop}, StateType::Standing},
        {{StateType::Sprinting, Transition::Move}, StateType::Running},
        {{StateType::Sprinting, Transition::Roll}, StateType::Rolling},
        {{StateType::Sprinting, Transition::Jump}, StateType::InAir},
        {{StateType::Sprinting, Transition::Fall}, StateType::Falling},
        {{StateType::Rolling, Transition::Stop}, StateType::Standing},
        {{StateType::Rolling, Transition::Fall}, StateType::Falling},
        {{StateType::Falling, Transition::Stop}, StateType::Standing},
        {{StateType::AttackAll, Transition::Stop}, StateType::Standing},
        {{StateType::Standing, Transition::AttackAll}, StateType::AttackAll}
    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Running, new Running()},
        {StateType::Sprinting, new Sprinting()},
        {StateType::Rolling, new Rolling()},
        {StateType::Falling, new Falling()},
        {StateType::AttackAll, new AttackAllEnemies()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};

    //player input
    Keyboard_Input* input = new Keyboard_Input();

    player = std::make_unique<GameObject>("player", fsm, input, Color{255,0,0,255});
}

void Game::update_enemy(GameObject& obj) {
    Transitions transitions;
    States states;
    //TODO change names
    if (obj.obj_name == "slime") {
        transitions = {
            {{StateType::Standing, Transition::Move}, StateType::Patrolling},
            {{StateType::Patrolling, Transition::Stop}, StateType::Standing},
            {{StateType::Standing, Transition::Fall}, StateType::InAir},
            {{StateType::InAir, Transition::Stop}, StateType::Patrolling}
        };
        states = {
            {StateType::Standing, new Standing()},
            {StateType::Patrolling, new Patrolling()},
            {StateType::InAir, new InAir()}
        };
    } else if (obj.obj_name == "bee") {
        transitions = {
            {{StateType::Standing, Transition::Move}, StateType::Patrolling},
            {{StateType::Patrolling, Transition::Stop}, StateType::Standing},
        };
        states = {
            {StateType::Standing, new Standing()},
            {StateType::Patrolling, new Patrolling()},
        };
    } else {
        throw std::runtime_error("dont know what that enemy is");
    }

    FSM* fsm = new FSM{transitions, states, StateType::Patrolling};
    obj.fsm = fsm;

    Input* input = new Ai_input{};
    input->next_action_type = ActionType::MoveRight;
    obj.input = input;
}



