#ifndef MONTECARLOSOLVER_H
#define MONTECARLOSOLVER_H

#include <iostream>
#include <chrono>
#include <numeric>
#include "Game.hpp"

using namespace std::chrono;

void monteCarloSolve(int n, int runs, int display_level, int win);
std::pair<int, int> monteCarloSimulateGame(int runs, int display_level);
std::pair<int, int> simulateOneRun(Game game);

#endif