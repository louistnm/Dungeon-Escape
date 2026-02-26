#pragma once
#include "fsm.h"

class Standing : public State {
public:
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override;

};

class Running : public State {
    void on_enter(World&, GameObject&) override;
    Action* input(World&, GameObject&, ActionType) override;
};

class InAir : public State {
public:
    void on_enter(World&, GameObject&) override;
    void update(World&, GameObject&, double dt) override;

    const double cooldown = 0.1; //seconds
    double elapsed = 0;
};