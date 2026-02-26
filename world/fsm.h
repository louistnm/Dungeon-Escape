//
// Created by Louis Tennier on 2/13/26.
//
#pragma once
#include <map>

class World;
class GameObject;
class Action;
enum class ActionType;
class State;

enum class StateType{Standing, InAir, Running}; //do stuff based off of appropriate things
enum class Transition{Jump, Stop, Move}; //key current state - going to next state

using Transitions = std::map<std::pair<StateType, Transition>, StateType>;
using States = std::map<StateType, State*>;

class FSM { //finite state machine
public:
    FSM(Transitions transitions, States states, StateType start); //have to have some state
    virtual ~FSM();

    void transition(Transition t, World& world, GameObject& obj); //watch single/plural

    //FSM data
    Transitions transitions;
    States states;
    StateType current_state_type;
    State* current_state;
};

class State {
public:
    virtual ~State() = default;

    virtual void on_enter(World&, GameObject&){}
    virtual void on_exit(World&, GameObject&){}

    virtual Action* input(World&, GameObject&, ActionType) {return nullptr;}
    virtual void update(World&, GameObject&, [[maybe_unused]]double dt){} //decorator that clears a warning


};


