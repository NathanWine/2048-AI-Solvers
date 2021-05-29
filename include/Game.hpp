#ifndef GAME_H
#define GAME_H

#include <vector>
#include <tuple>    // MAYBE REMOVE AND CHANGE TO OWN CLASS DEFINITION
#include <iostream>
#include <random>
#include <algorithm>
#include <string>


enum MOVES {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3};
typedef std::vector<std::vector<int>> board;
typedef std::vector<std::tuple<int, float, board>> weightedmoves;
const int DIM = 4;

extern std::random_device rd;
extern std::mt19937 rng;
extern std::uniform_int_distribution<> DISTS[];

class Game {
    protected:
        void addNew();
        void transpose();
        void mergeUp();
        void compressUp(bool peak);
        void hamburgerFlip();
    public: 
        int score;
        board state;

        Game() : score(0), state{{0, 0, 0, 0}, 
                                 {0, 0, 0, 0}, 
                                 {0, 0, 0, 0}, 
                                 {0, 0, 0, 0}} {
            addNew();
            addNew();
        }
        bool canContinue();
        std::vector<std::pair<int, board>> possibleMoves();
        weightedmoves computePossibilities();
        void up(bool peak);
        void left(bool peak);
        void right(bool peak);
        void down(bool peak);
        int getHighestTile();
        int getNumberEmpty();
        friend std::ostream& operator<<(std::ostream &stream, Game &game);
};

#endif