#ifndef MINIMAX_H
#define MINIMAX_H

#include "Game.hpp"

namespace Minimax {
    float minimaxScore(int depth, const Game &game);
    std::pair<int, int> minimaxSearch(int depth, int display_level, Game game);
    int calculateBestMove(int depth, int display_level, const Game &game);
    int minimaxSolve(int n, int depth, int display_level,
        std::vector<int> &scores, std::vector<int> &highest_tiles);
};

#endif