#include "Heuristics.hpp"

float snake_weights[4][4] = {
    {10,    8,    7,    6.5},
    {0.5,   0.7,  1,    3},
    {-0.5, -1.5, -1.8, -2},
    {-3.8, -3.7  -3.5, -3}
};

float get_snake(Game game) {        // Perhaps this should be normalized and multiplied by a weight instead? Becomes the only factor at high value tiles
    float snake_total = 0.0;
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            snake_total += game.state[i][j] * snake_weights[i][j];
        }
    }
    return snake_total;
}

float get_empty(Game game, float weight) {
    return game.getNumberEmpty() * weight;
}

float get_h_score(Game game) {
    return get_snake(game) + std::pow(get_empty(game, 1.0), 2.0);
}