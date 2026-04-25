//
// Created by Louis Tennier on 4/25/26.
//

#pragma once
#include "input.h"

class Ai_input : public Input {
public:
    void get_input() override;
    void handle_input(World& world, GameObject& obj) override;
};