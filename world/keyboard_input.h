//
// Created by Louis Tennier on 2/26/26.
//

#pragma once
#include "input.h"
class Keyboard_Input : public Input {
public:
    Action* collect_discrete_event(SDL_Event* event) override;
    void get_input() override;
    void handle_input(World& world, GameObject& obj, double) override;

};