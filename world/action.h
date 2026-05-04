//
// Created by Louis Tennier on 2/13/26.
//

#pragma once

enum class ActionType {
    None, Jump, MoveLeft, MoveRight, SprintLeft, SprintRight, RollLeft, RollRight, AttackAll, Attack, ShootFireball
};

class World;
class GameObject;

class Action {
public:
    virtual ~Action() = default;
    virtual void perform(World& world, GameObject& obj) = 0;
};

class Jump : public Action {
    void perform(World&, GameObject& obj) override;
};

class MoveRight : public Action {
    void perform(World&, GameObject& obj) override;
};

class MoveLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class SprintRight : public Action {
    void perform(World&, GameObject& obj) override;
};

class SprintLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class RollRight : public Action {
    void perform(World&, GameObject& obj) override;
};

class RollLeft : public Action {
    void perform(World&, GameObject& obj) override;
};

class Attack : public Action {
    void perform(World& world, GameObject& obj) override;
};


class ShootFireball : public Action {
public:
    void perform(World& world, GameObject& obj) override;
};