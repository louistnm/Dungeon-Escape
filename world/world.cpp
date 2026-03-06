#include "world.h"
#include "game_object.h"
#include <algorithm>
#include "fsm.h"
#include "physics.h"
#include "states.h"
#include "keyboard_input.h"

World::World(int width, int height)
    : tilemap{width,height} {}

void World::add_platform(float x, float y, float width, float height) {
    for (int i{0}; i < height; ++i) {
        for (int j{0}; j < width; ++j) {
            tilemap(x+j, y+i) = Tile::Platform;
        }
    }
}

bool World::collides(const Vec<float>& position) const {
    int x = std::floor(position.x);
    int y = std::floor(position.y);
    return tilemap(x,y) == Tile::Platform;
}

GameObject* World::create_player() {
    // Create FSM
    Transitions transitions = {
        {{StateType::Standing, Transition::Jump}, StateType::InAir}, //if standing and jump, go to inair
        {{StateType::InAir, Transition::Stop}, StateType::Standing},
        {{StateType::Standing, Transition::Move}, StateType::Running},
        {{StateType::Running, Transition::Stop}, StateType::Standing},
        {{StateType::Running, Transition::Jump}, StateType::InAir},
        {{StateType::Standing, Transition::Sprint}, StateType::Sprinting},
        {{StateType::Running, Transition::Sprint}, StateType::Sprinting},
        {{StateType::Sprinting, Transition::Stop}, StateType::Standing},
        {{StateType::Sprinting, Transition::Move}, StateType::Running},
        {{StateType::Sprinting, Transition::Jump}, StateType::InAir}

    };
    States states = {
        {StateType::Standing, new Standing()},
        {StateType::InAir, new InAir()},
        {StateType::Running, new Running()},
        {StateType::Sprinting, new Sprinting()}
    };
    FSM* fsm = new FSM{transitions, states, StateType::Standing};

    //player input
    Keyboard_Input* input = new Keyboard_Input();

    player = std::make_unique<GameObject>(Vec<float>{1,1}, *this, fsm, input, Color{255, 0, 0, 255});
    return player.get();
}

void World::move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity) {
    // test for collisions on the bottom or top first
    if (collides(position) && collides({position.x+size.x, position.y})) {
        position.y = ceil(position.y);
        velocity.y = 0;
    } else if (collides({position.x, position.y+size.y}) && collides({position.x+size.x, position.y+size.y})){
        position.y = floor(position.y);
        velocity.y = 0;
    }
    // then test for collisions on the left and right sides
    if (collides(position) && collides({position.x, position.y+size.y})) {
        position.x = ceil(position.x);
        velocity.x = 0;
    } else if (collides({position.x+size.x, position.y}) && collides({position.x+size.x, position.y+size.y})){
        position.x = floor(position.x);
        velocity.x = 0;
    }
    // now test each corner
    if (collides(position)) { //bottom left corner
        float dx = ceil(position.x) - position.x;
        float dy = ceil(position.y) - position.y;

        if (dx > dy) {
            position.y = ceil(position.y);
            velocity.y = 0;
        } else {
            position.x = ceil(position.x);
            velocity.x = 0;
        }
    } else if (collides({position.x+size.x, position.y})) { //bottom right corner
        float dx = position.x - floor(position.x);
        float dy = ceil(position.y) - position.y;

        if (dx > dy) {
            position.y = ceil(position.y);
            velocity.y = 0;
        } else {
            position.x = floor(position.x);
            velocity.x = 0;
        }
    } else if (collides({position.x, position.y+size.y})) { //top left corner
        float dx = ceil(position.x) - position.x;
        float dy = position.y - floor(position.y);

        if (dx > dy) {
            position.y = floor(position.y);
            velocity.y = 0;
        } else {
            position.x = ceil(position.x);
            velocity.x = 0;
        }
    } else if (collides({position.x+size.x, position.y+size.y})) { //top right corner
        float dx =  position.x - floor(position.x);
        float dy =  position.y - floor(position.y);

        if (dx > dy) {
            position.y = floor(position.y);
            velocity.y = 0;
        } else {
            position.x = floor(position.x);
            velocity.x = 0;
        }
    }
}

void World::update(float dt) {
    // //currently only player

    // ... the code to update velocity and position
    auto position = player->physics.position;
    auto velocity = player->physics.velocity;
    auto acceleration = player->physics.acceleration;

    velocity += 0.5f * acceleration * dt;
    position += velocity*dt;
    velocity += 0.5f * acceleration * dt; //calculate half the velocity before position calculation and half after
    velocity.x *= physics.damping;

    velocity.x = std::clamp(velocity.x, -physics.terminal_velocity, physics.terminal_velocity);
    velocity.y = std::clamp(velocity.y, -physics.terminal_velocity, physics.terminal_velocity);

    // check for collisions in the world - x direction
    Vec<float> future_position{position.x, player->physics.position.y};
    Vec<float> future_velocity{velocity.x, 0};
    move_to(future_position, player->size, future_velocity);
    // now y direction after (maybe) moving in x
    future_velocity.y = velocity.y;
    future_position.y = position.y;
    move_to(future_position, player->size, future_velocity);
    // update the player position and velocity
    player->physics.position = future_position;
    player->physics.velocity = future_velocity;

}

