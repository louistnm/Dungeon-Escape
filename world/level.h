//
// Created by Louis Tennier on 3/22/26.
//

#pragma once

#include "vec.h"
#include "tilemap.h"

#include <string>
#include <map>

struct Sound;
using TileLocations = std::map<Vec<int>, std::string>;
using EnemyLocations = std::map<Vec<float>, std::string>;

class Level {
public:
    Level(std::string name);

    // data
    int width{0};
    int height{0};
    std::string name;

    std::vector<std::string> tile_filenames;
    std::map<std::string, Tile> tile_types;

    TileLocations tile_locations;
    Vec<int> player_spawn_location{-1, -1};
    EnemyLocations enemy_locations;

    std::vector<Sound> sounds;


};