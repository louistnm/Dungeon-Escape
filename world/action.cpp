//
// Created by Louis Tennier on 2/13/26.
//

#include "action.h"
#include "game_object.h"
#include "world.h"
#include <iostream>

#include "audio.h"

void Jump::perform(World& world, GameObject& obj) {
    obj.physics.velocity.y = obj.physics.jump_velocity;
    world.audio->play_sounds("jump");
}

void MoveRight::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = obj.physics.walk_acceleration;
}

void MoveLeft::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = -obj.physics.walk_acceleration;
}

void SprintRight::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = 1.5*obj.physics.walk_acceleration;
}

void SprintLeft::perform(World& world, GameObject& obj) {
    obj.physics.acceleration.x = -1.5*obj.physics.walk_acceleration;
}

void RollRight::perform(World& world, GameObject& obj) {
    obj.physics.velocity.x = 10 + obj.physics.velocity.x;
    world.audio->play_sounds("jump");
}

void RollLeft::perform(World& world, GameObject& obj) {
    obj.physics.velocity.x = -10 + obj.physics.velocity.x;
    world.audio->play_sounds("jump");
}

void Attack::perform(World& world, GameObject& obj) {
    //check if obj is in player attack range
    for (auto& enemy : world.game_objects) {
        if (enemy == world.player) continue;
        if (std::abs(enemy->physics.position.x - world.player->physics.position.x) <= world.player->attack_range && world.player->physics.position.x <= enemy->physics.position.x && std::abs(enemy->physics.position.y - world.player->physics.position.y) <= 1) {
            enemy->take_damage(world.player->damage);
            if (enemy->obj_name == "necromancer") {
                world.audio->play_sounds("necromancer");
            } else {
                world.audio->play_sounds("sword");
            }
        }
    }
}

// void ShootFireball::perform(World& world, GameObject& obj) {
//     std::cout << "ran\n";
//     auto fireball = dynamic_cast<Projectile*>(world.available_items["fireball"]());
//     fireball->physics.position = obj.physics.position;
//     world.projectiles.push_back(fireball);
// }

// make the fireball shooting sound
//world.audio->play_sounds("fireball");