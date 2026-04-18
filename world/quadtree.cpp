//
// Created by Louis Tennier on 4/17/26.
//


#include "quadtree.h"
#include "game_object.h"

bool AABB::contains(Vec<float> point) const {
    Vec<float> displacement = point - center;
    return std::abs(displacement.x) < half_dimension.x && std::abs(displacement.y) < half_dimension.y;
}

bool AABB::intersects(AABB other) const {
    Vec<float> displacement = other.center - center;
    return std::abs(displacement.x) < (half_dimension.x + other.half_dimension.x) && std::abs(displacement.y) < (half_dimension.y + other.half_dimension.y);
}

Quadtree::Quadtree(AABB boundary)
    : boundary{boundary}{}

void Quadtree::clear() {
    objects.clear();
    north_east = nullptr;
    north_west = nullptr;
    south_west = nullptr;
    south_east = nullptr;
}

bool Quadtree::insert(GameObject* obj) {
    //ignore objects not in this node
    if (!boundary.contains(obj->get_bounding_box().center)) {
        return false;
    }

    //if there is space add it
    if (objects.size() < NODE_CAPACITY && north_west == nullptr) { // checks if this boundary is a leaf
        objects.push_back(obj);
        return true;
    }

    // otherwise subdivide and insert into children
    if (north_west == nullptr) {
        subdivide();
    }

    return north_west->insert(obj) || north_east->insert(obj) || south_west->insert(obj) || south_east->insert(obj);
}

std::vector<GameObject*> Quadtree::query_range(AABB range) const {
    if (!boundary.intersects(range)) {
        return {};
    }

    // handle leaf node
    std::vector<GameObject*> results;
    if (north_west == nullptr) {
        std::copy_if(std::begin(objects), std::end(objects), std::back_inserter(results),
                [&](GameObject* obj) {return range.intersects(obj->get_bounding_box());} //lambda function for if any object intersects with player
        );
        return results;
    }

    // add objects from children (and check for corresponding collisions with gameobjects again)
    auto pts = north_west->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = north_east->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = south_west->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));
    pts = south_east->query_range(range);
    results.insert(std::end(results), std::begin(pts), std::end(pts));

    return results;
}

void Quadtree::subdivide() {
    Vec<float> half = boundary.half_dimension / 2.0f;

    north_west = std::make_shared<Quadtree>(AABB{{boundary.center.x - half.x, boundary.center.y + half.y}, half});
    north_east = std::make_shared<Quadtree>(AABB{{boundary.center.x + half.x, boundary.center.y + half.y}, half});
    south_west = std::make_shared<Quadtree>(AABB{{boundary.center.x - half.x, boundary.center.y - half.y}, half});
    south_east = std::make_shared<Quadtree>(AABB{{boundary.center.x + half.x, boundary.center.y - half.y}, half});

    for (auto obj : objects) {
        insert(obj);
    }
    objects.clear();
}