#ifndef MINIMAX_H
#define MINIMAX_H

#include <iostream>
#include <limits>
#include "Game.hpp"
#include "Heuristics.hpp"

float minimaxScore(int depth, Game game);
std::pair<int, int> minimaxSearch(int depth, int display_level, Game game);
int minimaxSolve(int n, int depth, int display_level,
    std::vector<int> *scores, std::vector<int> *highest_tiles);

#endif