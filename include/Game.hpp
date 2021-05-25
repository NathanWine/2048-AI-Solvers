#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>
#include <random>

enum MOVES {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3};
typedef std::vector<std::vector<int>> board;
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
        std::vector<int> possibleMoves();
        void up(bool peak);
        void left(bool peak);
        void right(bool peak);
        void down(bool peak);
        int getHighestTile();
        friend std::ostream& operator<<(std::ostream &stream, Game &game);
};

#endif