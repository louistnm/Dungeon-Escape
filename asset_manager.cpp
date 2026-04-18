//
// Created by Louis Tennier on 2/26/26.
//
#include "asset_manager.h"

#include <filesystem>
#include <fstream>
#include "game_object.h"
#include "random.h"



// helper function to convert a list of sprites to animated sprites
void convert_sprites(std::vector<Sprite>& sprites, Graphics& graphics, GameObject& obj, bool random_start) {
    for (auto& sprite : sprites) {
        auto first_location = sprite.location;
        sprite.filename = (std::filesystem::current_path() / "assets" / sprite.filename).string();
        sprite.texture_id = graphics.get_texture_id(sprite.filename);
        sprite.shift = {-sprite.size.x/2, -sprite.size.y}; // anchor sprite at bottom corner
        sprite.center = sprite.size / 2.0f;
        std::vector<Sprite> sprite_frames;
        for (int i = 0; i < sprite.number_of_frames; ++i) {
            sprite.location = {first_location.x + i * sprite.size.x, first_location.y};
            sprite_frames.push_back(sprite);
        }
        int starting_frame = 0;
        if (random_start) {
            starting_frame = randint(0, sprite.number_of_frames - 1);
        }
        obj.sprites[sprite.name] = AnimatedSprite{sprite_frames, sprite.dt_per_frame, starting_frame};
    }
}

void AssetManager::get_game_object_details(const std::string& name, Graphics& graphics, GameObject& obj, bool random_start) {
    auto path_start = std::filesystem::current_path() / "assets";
    auto path = path_start/ (name + ".json");

    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path.string());
    }

    nlohmann::json json;
    file >> json;

    // get the object's sprites
    std::vector<Sprite> sprites_from_json = json.at("sprites").get<std::vector<Sprite>>();
    convert_sprites(sprites_from_json, graphics, obj, random_start);

    // get the object's physics
    auto pos = obj.physics.position;
    Physics physics = json.at("physics").get<Physics>();
    obj.physics = physics;
    obj.physics.position = pos;

    //get the object size
    obj.size = json.at("size").get<Vec<float>>();

    //get combat info
    obj.health = json.at("health").get<int>();
    obj.max_health = json.at("max_health").get<int>();
    obj.damage = json.at("damage").get<int>();

    obj.set_sprite("idle");
}

void convert_to_tiles(Graphics& graphics, Level &level, std::vector<Tile>& tiles, const std::string& filename) {
    for (auto& tile : tiles) {
        tile.sprite.filename = (std::filesystem::current_path() / "assets" / tile.sprite.filename).string();
        tile.sprite.texture_id = graphics.get_texture_id(tile.sprite.filename);
        tile.sprite.shift = {-tile.sprite.size.x/2, -tile.sprite.size.y}; // anchor sprite at bottom corner
        tile.sprite.center = tile.sprite.size / 2.0f;
        tile.id = filename + ":" +tile.sprite.name;
        level.tile_types[tile.id] = tile;
    }
}

void AssetManager::get_level_details(Graphics &graphics, Level &level) {
    auto path_start = std::filesystem::current_path() / "assets";
    auto path = path_start / (level.name+ ".json");

    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path.string());
    }

    nlohmann::json json;
    file >> json;

    json.get_to(level);

    // get all tile details from the file(s)
    for (auto filename : level.tile_filenames) {
        auto tiles_path = path_start / filename;
        std::ifstream tile_file(tiles_path);
        if (!tile_file) {
            throw std::runtime_error("Could not open file: " + tiles_path.string());
        }
        nlohmann::json tile_json;
        tile_file >> tile_json;
        std::vector<Tile> tiles = tile_json.at("tiles").get<std::vector<Tile>>();
        convert_to_tiles(graphics, level, tiles, filename);
    }
}

void AssetManager::update_level_details(const Level& level) {
    auto path = std::filesystem::current_path() / "assets" / (level.name + ".json");

    std::ofstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path.string());
    }
    nlohmann::json j = level;
    file << std::setw(4) << j;
}
