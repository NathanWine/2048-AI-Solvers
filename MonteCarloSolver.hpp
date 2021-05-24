#ifndef MONTECARLOSOLVER_H
#define MONTECARLOSOLVER_H

#include "Game.hpp"
#include <iostream>

void simulate_n_games(int win = 2048);
std::pair<int, int> simulate_game();
std::pair<int, int> simulate_one_run(Game game);

#endif