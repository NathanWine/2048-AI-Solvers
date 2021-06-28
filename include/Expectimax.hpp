#ifndef EXPECTIMAX_H
#define EXPECTIMAX_H

#include <iostream>
#include <limits>
#include "Game.hpp"
#include "Heuristics.hpp"

float expectimaxScore(int depth, Game game, bool is_max);
std::pair<int, int> expectimaxSearch(int depth, int display_level, Game game);
int expectimaxSolve(int n, int depth, int display_level,
    std::vector<int> *scores, std::vector<int> *highest_tiles);

#endif
