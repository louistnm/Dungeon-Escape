//
// Created by Louis Tennier on 4/25/26.
//

#include "ai_input.h"

#include "world.h"
#include "game_object.h"
#include "fsm.h"
#include <iostream>

void AiInput::get_input() {

}

void AiInput::handle_input(World& world, GameObject& obj, double dt) {
    constexpr float epsilon = 1e-4;

    //check for colliding with left wall
    if (world.collides({obj.physics.position.x - epsilon, obj.physics.position.y + obj.size.y - epsilon})) {
        next_action_type = ActionType::MoveRight;
    }

    //check for right wall
    if (world.collides({obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y + obj.size.y - epsilon})) {
        next_action_type = ActionType::MoveLeft;
    }
    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);

    if (action) {
        action->perform(world, obj);
        delete action;
    }

}
