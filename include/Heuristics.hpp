#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <cmath>
#include "Game.hpp"

extern float snake_weights[4][4];

float get_snake(Game game);
float get_empty(Game game, float weight);
float get_score(Game game);

#endif