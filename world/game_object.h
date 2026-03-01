#pragma once
#include <utility>
#include "vec.h"
#include "graphics.h"
#include "physics.h"

class FSM; //don't need all of FSM, so forward-declaring saves time
class Input;
class World;

class GameObject {
public:

    GameObject(const Vec<float>& position, const Vec<float>& size, World& world, FSM* fsm, Input* input, Color color);
    ~GameObject();

    void update(World& world, double dt);


    std::pair<Vec<float>, Color> get_sprite() const;


    // Player data
    Vec<float> size;
    Physics physics;
    FSM* fsm;
    Input* input;
    Color color;
    Sprite sprite;
};