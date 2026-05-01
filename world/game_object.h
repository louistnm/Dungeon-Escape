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
class AABB;

using Sprites = std::map<std::string, AnimatedSprite>;

class GameObject {
public:

    GameObject(std::string name, FSM* fsm, Input* input, Color color = {255,255,0,255});
    ~GameObject();

    virtual void update(World& world, double dt);


    std::pair<Vec<float>, Color> get_sprite() const;
    void set_sprite(const std::string& next_sprite);

    AABB get_bounding_box();

    void take_damage(int attack_damage);

    bool flash_sprite() const;


    // Player data
    std::string obj_name;
    Physics physics;
    Vec<float> size;
    FSM* fsm;
    Input* input;
    Color color;
    Sprites sprites;
    Sprite sprite;
    std::string sprite_name;

    //combat
    int health;
    int max_health;
    int damage;
    Vec<int> detection_distance;
    double iframe_time{0.0};

    bool is_alive{true};
    bool taken_damage{false};
    bool invulnerable{false};
    int damage_direction = -1; // 0 is for hit from left, 1 is for hit from right
};