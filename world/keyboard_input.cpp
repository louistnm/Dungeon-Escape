//
// Created by Louis Tennier on 2/26/26.
//

#include "keyboard_input.h"
#include "game_object.h"
#include "fsm.h"

void Keyboard_Input::get_input() {
    if (next_action_type == ActionType::Jump || next_action_type == ActionType::AttackAll || next_action_type == ActionType::RollLeft || next_action_type == ActionType::RollRight) return;
    const bool *key_states = SDL_GetKeyboardState(NULL);
    if (key_states[SDL_SCANCODE_A]) {
        next_action_type = ActionType::MoveLeft;
    }
    if (key_states[SDL_SCANCODE_D]) {
        next_action_type = ActionType::MoveRight;
    }
    if (key_states[SDL_SCANCODE_LSHIFT]){
        if (key_states[SDL_SCANCODE_A]) {
            next_action_type = ActionType::SprintLeft;
        } else if (key_states[SDL_SCANCODE_D]) {
            next_action_type = ActionType::SprintRight;
        }
    }

}

void Keyboard_Input::handle_input(World& world, GameObject& obj) {
    Action* action = obj.fsm->current_state->input(world, obj, next_action_type);

    //consume action
    next_action_type = ActionType::None;
    if (action != nullptr) {
        action->perform(world, obj);
        delete action; // after action is completed, delete the pointer
    }
}

Action* Keyboard_Input::collect_discrete_event(SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.repeat == 0) {
        if (event->key.scancode == SDL_SCANCODE_SPACE) {
            next_action_type = ActionType::Jump;
        }
        if (event->key.scancode == SDL_SCANCODE_Q) {
            next_action_type = ActionType::RollLeft;
        }
        if (event->key.scancode == SDL_SCANCODE_E) {
            next_action_type = ActionType::RollRight;
        }
        if (event->key.scancode == SDL_SCANCODE_M) {
            next_action_type = ActionType::AttackAll;
        }

    }
    return nullptr;
}

//fireball code
// if (event->key.scancode == SDL_SCANCODE_F) {
//     return new ShootFireball();
// }