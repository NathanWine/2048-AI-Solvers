#include "Heuristics.hpp"

const float snake_weights[4][4] = {
    {10,    8,    7,    6.5},
    {0.5,   0.7,  1,    3},
    {-0.5, -1.5, -1.8, -2},
    {-3.8, -3.7  -3.5, -3}
};

float Heuristics::get_snake(const board &state) {        // Perhaps this should be normalized and multiplied by a weight instead? Becomes the only factor at high value tiles
    float snake_total = 0.0;
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            snake_total += state[i][j] * snake_weights[i][j];
        }
    }
    return snake_total;
}

float Heuristics::get_empty(const board &state, float weight=1.0) {
    return Game::getNumberEmpty(state) * weight;
}

float Heuristics::get_h_score(const board &state) {
    return get_snake(state) + std::pow(get_empty(state), 2.0);
}