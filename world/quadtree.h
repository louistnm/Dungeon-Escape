//
// Created by Louis Tennier on 4/17/26.
//

#pragma once
#include <memory>
#include <vector>
#include "vec.h"

class GameObject;

struct AABB {
    Vec<float> center;
    Vec<float> half_dimension;

    bool contains(Vec<float> point) const;
    bool intersects(AABB other) const;
};

class Quadtree {
public:
    Quadtree(AABB boundary);

    void clear(); //remove children and any objects stored

    bool insert(GameObject* obj);
    std::vector<GameObject*> query_range(AABB range) const;

    void subdivide();

    // how many elements can be stored
    static constexpr std::size_t NODE_CAPACITY = 4;

    //boundary of this node
    AABB boundary;

    //objects in this node
    std::vector<GameObject*> objects;

    //children
    std::shared_ptr<Quadtree> north_west, north_east, south_west, south_east;
};