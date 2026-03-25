#pragma once
#include <utility>
#include "vec.h"
#include "graphics.h"
#include "physics.h"
#include <map>
#include "animated_sprite.h"

class FSM; //don't need all of FSM, so forward-declaring saves time
class Input;
class World;

using Sprites = std::map<std::string, AnimatedSprite>;

class GameObject {
public:

    GameObject(const Vec<float>& position, const Vec<float>& size, World& world, FSM* fsm, Input* input, Color color);
    ~GameObject();

    void update(World& world, double dt);


    std::pair<Vec<float>, Color> get_sprite() const;
    void set_sprite(const std::string& next_sprite);


    // Player data
    Vec<float> position;
    Vec<float> size;
    Physics physics;
    FSM* fsm;
    Input* input;
    Color color;
    Sprites sprites;
    Sprite sprite;
    std::string sprite_name;
};