//
// Created by Louis Tennier on 2/19/26.
//

#include "states.h"
#include "action.h"
#include "game_object.h"
#include "random.h"
#include "world.h"
#include <iostream>


//Helper function
bool on_platform(World& world, GameObject& obj) {
    constexpr float epsilon = 1e-4;
    Vec<float> left_foot{obj.physics.position.x + epsilon, obj.physics.position.y - epsilon};
    Vec<float> right_foot{obj.physics.position.x + obj.size.x - epsilon, obj.physics.position.y - epsilon};
    return world.collides(left_foot) || world.collides(right_foot);
}


//////////
//Standing
//////////

void Standing::on_enter(World&, GameObject& obj) {
    obj.color = {255, 0, 0, 255}; //change color to visualize different states
    obj.set_sprite("idle");
    obj.physics.acceleration.x = 0;
}

Action* Standing::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump(); //new jump action
    } else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight();
    } else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft();
    } else if (action_type == ActionType::SprintRight) {
        obj.fsm->transition(Transition::Sprint, world, obj);
        return new SprintRight();
    } else if (action_type == ActionType::SprintLeft) {
        obj.fsm->transition(Transition::Sprint, world, obj);
        return new SprintLeft();
    } else if (action_type == ActionType::RollRight) {
        obj.fsm->transition(Transition::Roll, world, obj);
        return new RollRight();
    } else if (action_type == ActionType::RollLeft) {
        obj.fsm->transition(Transition::Roll, world, obj);
        return new RollLeft();
    } else if (action_type == ActionType::AttackAll) {
        obj.fsm->transition(Transition::AttackAll, world, obj);
    } else if (obj.taken_damage) {
        obj.fsm->transition(Transition::Hit, world, obj);
    }
    return nullptr;
}

void Standing::update(World& world, GameObject& obj, double) {
    if (!on_platform(world, obj)) {
        obj.fsm->transition(Transition::Fall, world, obj);
    }
}

///////
///Waiting
///////

void Waiting::on_enter(World& world, GameObject& obj) {
    //set cooldown to a random amount of time
    elapsed = 0;
    cooldown = randint(2,4);
    Standing::on_enter(world, obj);
}

Action* Waiting::input(World& world, GameObject& obj, ActionType action_type) {
    if (elapsed >= cooldown) {
        return Standing::input(world, obj, action_type);
    }
    //check when player is left of enemy and within detection distance
    if (world.player->physics.position.x + 10 >= obj.physics.position.x && world.player->physics.position.x < obj.physics.position.x) {
        obj.fsm->transition(Transition::Target, world, obj);
    }

    //check when player is right of enemy and within detection distance
    if (world.player->physics.position.x - 10 <= obj.physics.position.x && world.player->physics.position.x > obj.physics.position.x) {
        obj.fsm->transition(Transition::Target, world, obj);
    }
    return Standing::input(world, obj, ActionType::None);
}

void Waiting::update(World&, GameObject&, double dt) {
    elapsed += dt;
}

//////////
// InAir
//////////

void InAir::on_enter(World&, GameObject& obj) {
    elapsed = cooldown;
    obj.color = {0, 0, 255, 255};
    obj.set_sprite("jumping");

}


void InAir::update(World& world, GameObject& obj, double dt) {
    elapsed -= dt;
    if (obj.taken_damage) {
        obj.fsm->transition(Transition::Hit, world, obj);
    }
    if (elapsed <= 0 && on_platform(world,obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

///////
/// Running
///////

void Running::on_enter(World&, GameObject& obj) {
    obj.color = {255, 255, 0, 255};
    obj.set_sprite("running");
}

Action* Running::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    } else if (action_type == ActionType::SprintRight) {
        obj.fsm->transition(Transition::Sprint, world, obj);
        return new SprintRight();
    } else if (action_type == ActionType::SprintLeft) {
        obj.fsm->transition(Transition::Sprint, world, obj);
        return new SprintLeft();
    } else if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump(); //starts physics of jumping
    } else if (action_type == ActionType::RollRight && obj.physics.velocity.x >= 0) {
        obj.physics.velocity.x *= 0.75;
        obj.fsm->transition(Transition::Roll, world, obj);
        return new RollRight();
    } else if (action_type == ActionType::RollLeft && obj.physics.velocity.x <= 0) {
        obj.physics.velocity.x *= 0.75;
        obj.fsm->transition(Transition::Roll, world, obj);
        return new RollLeft();
    } else if (obj.taken_damage) {
        obj.fsm->transition(Transition::Hit, world, obj);
    }
    return nullptr;
}

void Running::update(World& world, GameObject& obj, double) {
    if (!on_platform(world, obj)) {
        if (obj.physics.acceleration.x > 0) {
            obj.physics.acceleration.x = 0.5*obj.physics.walk_acceleration;
        } else if (obj.physics.acceleration.x > 0) {
            obj.physics.acceleration.x = -0.5*obj.physics.walk_acceleration;
        }
        obj.fsm->transition(Transition::Fall, world, obj);
    }
}

///////
///Patrolling
///////
void Patrolling::on_enter(World& world, GameObject& obj) {
    //set cooldown to a random amount of time
    elapsed = 0;
    cooldown = randint(4,6);
    Running::on_enter(world, obj);
}

Action* Patrolling::input(World& world, GameObject& obj, ActionType action_type) {
    constexpr float epsilon = 1e-4;
    if (elapsed >= cooldown) {
        return Running::input(world, obj, ActionType::None);
    } else if (world.collides({obj.physics.position.x - epsilon, obj.physics.position.y + obj.size.y - epsilon})) {
        return Running::input(world, obj, ActionType::None); //collided left wall
    } else if (world.collides({obj.physics.position.x + obj.size.x + epsilon, obj.physics.position.y + obj.size.y - epsilon})) {
        return Running::input(world, obj, ActionType::None); //collided right wall
    }
    //check when player is left of enemy and within detection distance
    if (world.player->physics.position.x + 10 >= obj.physics.position.x && world.player->physics.position.x < obj.physics.position.x) {
        obj.fsm->transition(Transition::Target, world, obj);
    }

    //check when player is right of enemy and within detection distance
    if (world.player->physics.position.x - 10 <= obj.physics.position.x && world.player->physics.position.x > obj.physics.position.x) {
        obj.fsm->transition(Transition::Target, world, obj);
    }
    return Running::input(world, obj, action_type);
}

void Patrolling::update(World&, GameObject&, double dt) {
    elapsed += dt;
}

////////
///Sprinting
////////

void Sprinting::on_enter(World&, GameObject& obj) {
    obj.color = {0, 255, 255, 255};
    obj.set_sprite("sprinting");
}

Action* Sprinting::input(World& world, GameObject& obj, ActionType action_type) {
    if (action_type == ActionType::None) {
        obj.fsm->transition(Transition::Stop, world, obj);
    } else if (action_type == ActionType::MoveRight) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveRight;
    } else if (action_type == ActionType::MoveLeft) {
        obj.fsm->transition(Transition::Move, world, obj);
        return new MoveLeft;
    } else if (action_type == ActionType::Jump) {
        obj.fsm->transition(Transition::Jump, world, obj);
        return new Jump();
    } else if (action_type == ActionType::RollRight && obj.physics.velocity.x >= 0) {
        obj.physics.velocity.x *= 0.5;
        obj.fsm->transition(Transition::Roll, world, obj);
        return new RollRight();
    } else if (action_type == ActionType::RollLeft  && obj.physics.velocity.x <= 0) {
        obj.physics.velocity.x *= 0.5;
        obj.fsm->transition(Transition::Roll, world, obj);
        return new RollLeft();
    } else if (obj.taken_damage) {
        obj.fsm->transition(Transition::Hit, world, obj);
    }
    return nullptr;
}

void Sprinting::update(World& world, GameObject& obj, double) {
    if (!on_platform(world, obj)) {
        if (obj.physics.acceleration.x > 0) {
            obj.physics.acceleration.x = 0.75*obj.physics.walk_acceleration;
        } else if (obj.physics.acceleration.x > 0) {
            obj.physics.acceleration.x = -0.75*obj.physics.walk_acceleration;
        }
        obj.fsm->transition(Transition::Fall, world, obj);
    }
}

////////
///Rolling
////////

void Rolling::on_enter(World&, GameObject& obj) {
    elapsed = cooldown;
    obj.color = {0, 0, 0, 255};
    obj.set_sprite("rolling");
    obj.invulnerable = true;
}

void Rolling::update(World& world, GameObject& obj, double dt) {
    elapsed -= dt;
    if (elapsed <= 0 && !on_platform(world, obj)) {
        obj.invulnerable = false;
        obj.fsm->transition(Transition::Fall, world, obj);
    } else if (elapsed <= 0 && on_platform(world, obj)) {
        obj.invulnerable = false;
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

////////
///Falling
////////

void Falling::on_enter(World&, GameObject& obj) {
    obj.physics.velocity.x *= 1/10;
    elapsed = cooldown;
    obj.color = {255, 255, 255, 255};
    obj.set_sprite("falling");
}

void Falling::update(World& world, GameObject& obj, double dt) {
    elapsed -= dt;
    if (obj.taken_damage) {
        obj.fsm->transition(Transition::Hit, world, obj);
    }
    if (elapsed <= 0 && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

///////
///AttackAll
///////

void AttackAllEnemies::on_enter(World& world, GameObject& obj) {
    obj.color = {255,100,0,255};
    for (auto& enemy : world.game_objects) {
        if (enemy == world.player) continue;
        enemy->take_damage(obj.damage);
    }
    elapsed = 0;
}

void AttackAllEnemies::update(World& world, GameObject& obj, double dt) {
    elapsed += dt;
    if (obj.taken_damage) {
        obj.fsm->transition(Transition::Hit, world, obj);
    }
    if (elapsed >= cooldown) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

////////
///Targeting
////////

void Targeting::on_enter(World& world, GameObject& obj) {
    obj.color = {0, 0, 0, 255};
}

void Targeting::update(World& world, GameObject& obj, double dt) {
    if (elapsed >= cooldown) {
        // check if player is still within detection range
        obj.set_sprite("attacking");
        //check when player is left of enemy and within detection distance
        if (world.player->physics.position.x + obj.detection_distance.x >= obj.physics.position.x && world.player->physics.position.x < obj.physics.position.x && std::abs(world.player->physics.position.y-obj.physics.position.y ) <= obj.detection_distance.y) {
            obj.physics.acceleration.x = -obj.physics.walk_acceleration *1.5;
            elapsed = 0;
        }

        //check when player is right of enemy and within detection distance
        else if (world.player->physics.position.x - obj.detection_distance.x <= obj.physics.position.x && world.player->physics.position.x > obj.physics.position.x && std::abs(world.player->physics.position.y-obj.physics.position.y ) <= obj.detection_distance.y) {
            obj.physics.acceleration.x = obj.physics.walk_acceleration * 1.5;
            elapsed = 0;
        }

        else {
            obj.fsm->transition(Transition::Stop, world, obj);
        }
    }
    elapsed += dt;
}

////////
///Knocked
////////

void Knocked::on_enter(World& world, GameObject& obj) {
    obj.set_sprite("falling");
    obj.taken_damage = false;
    obj.color = {170, 170, 0, 255};
    elapsed = 0;
    if (obj.damage_direction == 0) {
        obj.physics.velocity = {10,10};
    } else if (obj.damage_direction == 1) {
        obj.physics.velocity = {-10,10};
    }
}

void Knocked::update(World& world, GameObject& obj, double dt) {
    elapsed += dt;
    if (!on_platform(world, obj) && elapsed >= cooldown) {
        obj.fsm->transition(Transition::Fall, world, obj);
    }
}


