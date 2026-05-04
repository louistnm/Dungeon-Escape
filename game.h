#pragma once
#include "world.h"
#include "camera.h"
#include "audio.h"
#include "graphics.h"

enum class GameMode{Playing, GameOver, Win};

class Game {
public:
    Game(std::string title, int width, int height);
    ~Game();
    void handle_event(SDL_Event* event);
    void input();
    void update();
    void render();

private:
    std::unique_ptr<GameObject> player;
    World* world = nullptr;
    Graphics graphics;
    Camera camera;
    Audio audio;

    //timing
    float dt;
    Uint64 performance_frequency;
    Uint64 prev_counter;
    float lag;

    //events
    std::map<std::string, Event*> events;
    void get_events();

    //level help
    void create_player();
    int current_level{0};
    void load_level();
    void update_enemy(GameObject& obj);
    bool final_level{false};

    //game state
    GameMode mode{GameMode::Playing};
};