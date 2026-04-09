//
// Created by Louis Tennier on 2/19/26.
//

#include "states.h"
#include "action.h"
#include "game_object.h"
#include "world.h"

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
    }
    return nullptr;
}

void Standing::update(World& world, GameObject& obj, double) {
    if (!on_platform(world, obj)) {
        obj.fsm->transition(Transition::Fall, world, obj);
    }
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
}

void Rolling::update(World& world, GameObject& obj, double dt) {
    elapsed -= dt;
    if (elapsed <= 0 && !on_platform(world, obj)) {
        obj.fsm->transition(Transition::Fall, world, obj);
    } else if (elapsed <= 0 && on_platform(world, obj)) {
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
    if (elapsed <= 0 && on_platform(world, obj)) {
        obj.fsm->transition(Transition::Stop, world, obj);
    }
}

////////
///Tumbling
////////

////////
///Knocked
////////

////////
///Dizzy
////////




