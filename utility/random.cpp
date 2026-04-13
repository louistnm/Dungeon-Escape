//
// Created by Louis Tennier on 4/10/26.
//

#include "random.h"
#include <random>

int randint(int min , int max ) {
    static std::random_device rd;
    static std::default_random_engine ran{rd()};
    return std::uniform_int_distribution<>{min, max}(ran);
}