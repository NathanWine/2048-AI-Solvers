#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <cmath>
#include "Game.hpp"

// extern const float snake_weights[4][4];
namespace Heuristics {
    float get_snake(const board &state);
    float get_empty(const board &state, float weight);
    float get_h_score(const board &state);
}

#endif