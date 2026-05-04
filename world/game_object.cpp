//
// Created by Louis Tennier on 2/11/26.
//

#include "game_object.h"

#include "physics.h"
#include "fsm.h"
#include "input.h"
#include "quadtree.h"

GameObject::GameObject(std::string name, FSM* fsm, Input* input, Color color)
    : obj_name{name}, fsm{fsm}, input{input}, color{color} {}

GameObject::~GameObject() {
    delete fsm;
    delete input;
}

void GameObject::update(World& world, double dt) {
    if (fsm != nullptr) {
        fsm->current_state->update(world, *this, dt);
    }
    sprites[sprite_name].update(dt);
    sprites[sprite_name].flip(physics.acceleration.x < 0);
    set_sprite(sprite_name);

    iframe_time -= dt;

    if (iframe_time <= 0.0) {
        taken_damage = false;
    }
}

std::pair<Vec<float>, Color> GameObject::get_sprite() const {
    return {physics.position, color};
}

void GameObject::set_sprite(const std::string& next_sprite) {
    //if the next sprite is a new sprite, reset current sprite
    if (next_sprite != sprite_name) {
        sprites[sprite_name].reset(); //set it to frame = 0
        //default all sprites to idle
        auto itr = sprites.find(next_sprite);
        if (itr != sprites.end()) {
            sprite_name = next_sprite;
        } else {
            sprite_name = "idle";
        }
    }

    sprite = sprites[sprite_name].get_sprite();

}

AABB GameObject::get_bounding_box() {
    Vec<float> half_size = {size.x / 2.0f, size.y / 2.0f};
    Vec<float> center = {physics.position.x + half_size.x, physics.position.y + half_size.y};
    return {center, half_size};
}

void GameObject::take_damage(int attack_damage) {
    if (iframe_time > 0.0) return;
    if (invulnerable) return;
    health -= attack_damage;
    iframe_time = 1.5;
    taken_damage = true;
    if (health <= 0){
        is_alive = false;
    }
}

bool GameObject::flash_sprite() const {
    if (iframe_time <= 0.0) {
        return false;
    }
    // alternate overlay on/off every 80 ms
    return ((SDL_GetTicks() / 80) % 2) == 0;
}