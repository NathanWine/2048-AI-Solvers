#ifndef MONTECARLOSOLVER_H
#define MONTECARLOSOLVER_H

#include "Game.hpp"
#include <iostream>
#include <chrono>

using namespace std::chrono;

extern std::uniform_int_distribution<> a;
extern std::uniform_int_distribution<> b;
extern std::uniform_int_distribution<> c;
extern std::uniform_int_distribution<> d;
extern std::uniform_int_distribution<> DISTS[];

void monteCarloSolve(int n, int runs, int win);
std::pair<int, int> monteCarloSimulateGame(int runs);
std::pair<int, int> simulateOneRun(Game game);

#endif