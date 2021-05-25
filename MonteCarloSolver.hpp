#ifndef MONTECARLOSOLVER_H
#define MONTECARLOSOLVER_H

#include "Game.hpp"
#include <iostream>
#include <chrono>

using namespace std::chrono;

void monteCarloSolve(int n, int runs, int win);
std::pair<int, int> monteCarloSimulateGame(int runs);
std::pair<int, int> simulateOneRun(Game game);

#endif