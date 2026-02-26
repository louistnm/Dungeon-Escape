//
// Created by Louis Tennier on 2/26/26.
//
#pragma once
#include "vec.h"
#include "sprite.h"
#include "json.hpp"
#include "graphics.h"

// need to map Vec to JSON
template <typename T>
void to_void(const nlohmann::json& j, const Vec<T> v) {
    j = nlohmann::json::array({v.x,v.y});
}

template <typename T>
void from_json(const nlohmann::json& j, Vec<T>& v) {
    v.x = j.at(0).get<T>();
    v.y = j.at(1).get<T>();
}

//needed for JSON conversions
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Sprite, filename, location, size, scale); //auto handles to and from json for the class
namespace AssetManager {
    Sprite get_game_object_sprite(const std::string& name, Graphics& graphics);
}