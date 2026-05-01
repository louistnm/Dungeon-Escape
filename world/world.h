#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include <vector>
#include "tilemap.h"
#include "vec.h"
#include "game_object.h"
#include "quadtree.h"
#include "projectile.h"

class Level;
class Audio;
class Event;

class World {
public:
    World(const Level& level, Audio& audio, GameObject* player, std::map<std::string, Event*> events);
    ~World();
    void add_platform(float x, float y, float width, float height);
    void move_to(Vec<float>& position, const Vec<float>& size, Vec<float>& velocity);
    void update(double dt);
    void update_object(GameObject*, double dt);
    void load_level(const Level& level);
    void touch_tiles(GameObject& obj);
    void build_quadtree();

    bool end_level{false};
    bool end_game{false};
    bool collides(const Vec<float>& position) const;

    Tilemap tilemap;
    GameObject* player;
    Audio* audio;
    std::vector<GameObject*> game_objects;
    std::vector<Projectile*> projectiles;
    std::vector<Background> backgrounds;

    std::map<std::string, Event*> events;

    Quadtree quadtree;

    //items
    std::map<std::string, std::function<GameObject*()>> available_items;
};