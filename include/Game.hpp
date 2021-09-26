#ifndef GAME_H
#define GAME_H

#include <vector>
#include <map>
#include <iostream>
#include <random>
#include <algorithm>
#include <string>

class Game;
enum MOVES {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3};
typedef std::vector<std::vector<int>> board;
typedef std::vector<std::pair<float, Game>> weightedmoves;
typedef std::vector<std::pair<int, Game>> movelist;
const int DIM = 4;
const int WIN = 2048;

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
        bool canContinue() const;
        bool generatePossibleMoves(movelist &moves) const;
        std::map<int, weightedmoves> computePossibilities() const;
        void up(bool peak);
        void left(bool peak);
        void right(bool peak);
        void down(bool peak);
        int getHighestTile() const;
        static int getNumberEmpty(const board &state);
        static const std::string staticPrint(const Game &game, int *width);
        const std::string printBoard(int *width) const;
        friend std::ostream& operator<<(std::ostream &stream, const Game &game);
};

#endif