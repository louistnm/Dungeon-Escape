#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include <vector>
#include "tilemap.h"
#include "vec.h"
#include "game_object.h"

class Level;
class Audio;

class World {
public:
    World(const Level& level, Audio& audio);

    void add_platform(float x, float y, float width, float height);
    bool collides(const Vec<float>& position) const;
    GameObject* create_player(const Level& level);
    void move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity);
    void update(float dt);
    void load_level(const Level& level);

    Tilemap tilemap;

private:
    GameObject* player;
    Audio* audio;

};