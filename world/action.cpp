//
// Created by Louis Tennier on 2/13/26.
//

#include "action.h"
#include "game_object.h"
#include "world.h"

void Jump::perform(World&, GameObject& obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
}

void MoveRight::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = obj.physics.walk_acceleration;
}

void MoveLeft::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
}

void SprintRight::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = 1.5*obj.physics.walk_acceleration;
}

void SprintLeft::perform(World&, GameObject& obj) {
    obj.physics.acceleration.x = -1.5*obj.physics.walk_acceleration;
}

void RollRight::perform(World&, GameObject& obj) {
    obj.physics.velocity.x = 10 + obj.physics.velocity.x;
}

void RollLeft::perform(World&, GameObject& obj) {
    obj.physics.velocity.x = -10 + obj.physics.velocity.x;
}

//fireball code
// void ShootFireball::perform(World& world, GameObject& obj) {
//     auto fireball = dynamic_cast<Projectile*>(world.available_items["fireball"]());
//     fireball->physics.position = obj.physics.position;
//     world.projectiles.push_back(fireball);
// }