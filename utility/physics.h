#pragma once
#include "vec.h"
class Physics {
public:
    Physics()=default;
    Physics(Vec<float> position, Vec<float> velocity, Vec<float> acceleration);
    Physics(Vec<float> position, Vec<float> velocity, Vec<float> acceleration,
    float gravity, float damping, float walk_acceleration, float jump_velocity, float terminal_velocity);
    Vec<float> position{0,0};
    Vec<float> velocity{0,0};
    Vec<float> acceleration{0,0};
    // note that these are my default values for my player's physics. You should update these to your prefer
    float gravity{-30.0f};
    float damping{0.9f};
    float walk_acceleration{30.0f};
    float jump_velocity{20.0f};
    float terminal_velocity{15.0f};

};