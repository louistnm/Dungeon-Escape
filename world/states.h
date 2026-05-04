#pragma once
#include "fsm.h"

class Standing : public State {
public:
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override;
    void update(World&, GameObject&, double) override;

    double elapsed{0.0};
    double cooldown{0.0};
};

class Waiting : public Standing {
public:
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override;
    void update(World&, GameObject&, double dt) override;

    double elapsed{0.0};
    double cooldown{0.0};
};

class Running : public State {
public:
    virtual void on_enter(World&, GameObject&) override;
    virtual Action* input(World&, GameObject&, ActionType) override;
    virtual void update(World&, GameObject&, double) override;

    double elapsed{0.0};
    double cooldown{0.0};
};

class Patrolling : public Running {
public:
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override;
    void update(World&, GameObject&, double dt) override;

    double elapsed{0.0};
    double cooldown{0.0};
};

class InAir : public State {
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override;

    const double cooldown = 0.1; //seconds
    double elapsed = 0;
};

class Sprinting : public State {
public:
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override;
    void update(World&, GameObject&, double) override;
};

class Rolling : public State {
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override;

    const double cooldown = 0.5; // seconds
    double elapsed = 0;
};

class Falling : public State { //Falling State in addition to inair state for different sprites
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override;

    const double cooldown = 0.4;
    double elapsed = 0;
};

class AttackAllEnemies : public State {
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override;

    double elapsed = 0;
    double cooldown = 2;
};

class Attacking : public State {
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override;

    double elapsed = 0;
    double cooldown = 0.5;
};

class Targeting : public State {
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override;

    double elapsed = 0;
    double cooldown = 0.5;
};

class Knocked : public State {
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override;

    double elapsed = 0;
    double cooldown = 0.5;
};
