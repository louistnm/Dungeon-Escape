//
// Created by Louis Tennier on 2/26/26.
//
#pragma once
#include "action.h"
#include "SDL3/SDL.h"

class GameObject;

class Input {
public:
    virtual ~Input() = default;

    virtual Action* collect_discrete_event(SDL_Event* event) {return nullptr;}
    virtual void get_input() = 0;
    virtual void handle_input(World& world, GameObject& obj, double dt) = 0;
    ActionType next_action_type = ActionType::None;
};