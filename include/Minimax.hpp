#ifndef MINIMAX_H
#define MINIMAX_H

#include <iostream>
#include <chrono>
#include <limits>
#include <numeric>
#include "Game.hpp"
#include "Heuristics.hpp"

using namespace std::chrono;

void minimaxSolve(int n, int depth, int display_level, int win);
std::pair<int, int> minimaxSearch(int depth, int display_level);
float minimaxScore(int depth, Game game);

#endif