//
// Created by Louis Tennier on 2/26/26.
//

#include "asset_manager.h"

#include <filesystem>
#include <fstream>

Sprite AssetManager::get_game_object_sprite(const std::string& name, Graphics& graphics) {
    auto path_start = std::filesystem::current_path() / "assets";
    auto path = path_start / (name+ ".json");

    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path.string());
    }
    nlohmann::json json;
    file >> json;

    Sprite s = json.at("idle").get<Sprite>();
    s.filename = (path_start / s.filename).string();
    int texture_id = graphics.get_texture_id(s.filename);
    s.texture_id = texture_id;
    s.shift = {-s.size.x/2, -s.size.y};
    s.center = s.size / 2.0f;
    return s;
}
