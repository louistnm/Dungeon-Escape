//
// Created by Louis Tennier on 4/25/26.
//

#pragma once

#include "input.h"

class AiInput : public Input {
public:
    void get_input() override;
    void handle_input(World& world, GameObject& obj, double dt) override;
};