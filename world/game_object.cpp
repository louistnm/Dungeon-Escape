//
// Created by Louis Tennier on 2/11/26.
//

#include "game_object.h"
#include "physics.h"
#include "fsm.h"
#include "action.h"

GameObject::GameObject(const Vec<float>& position, const Vec<float>& size, World& world, FSM* fsm, Color color)
    : physics{position, {0,0}, {0,0}}, size{size}, fsm{fsm}, color{color} {
    physics.acceleration.y = physics.gravity;
    fsm->current_state->on_enter(world, *this);
}

GameObject::~GameObject() {
    delete fsm;
}

void GameObject::input(World& world) {
    const bool *key_states = SDL_GetKeyboardState(NULL);

    ActionType action_type = ActionType::None;
    if (key_states[SDL_SCANCODE_A] && key_states[SDL_SCANCODE_D]) {
        action_type = ActionType::None;
    } else {
        if (key_states[SDL_SCANCODE_A]) {
            action_type = ActionType::MoveLeft;
        }
        if (key_states[SDL_SCANCODE_D]) {
            action_type = ActionType::MoveRight;
        }
    }

    if (key_states[SDL_SCANCODE_SPACE]) {
        action_type = ActionType::Jump;
    }
    Action* action = fsm->current_state->input(world, *this, action_type);
    if (action != nullptr) {
        action->perform(world, *this);
        delete action; // after action is completed, delete the pointer
    }
}

void GameObject::update(World& world, double dt) {
    fsm->current_state->update(world, *this, dt);{}
}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, color};
}