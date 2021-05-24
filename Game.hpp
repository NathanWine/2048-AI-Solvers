#include <vector>
#include <iostream>
#include <random>

enum MOVES {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3};
typedef std::vector<std::vector<int>> board;

class Game {
    protected:
        board AddNew(board m);
        board Transpose(board m);
        board MergeUp(board m);
        board CompressUp(board m, bool peek);
        board HamburgerFlip(board m);
    public: 
        board state;
        int score;
        int highest_tile;
        const int DIM;

        Game(int d=4) : DIM(d), score(0), highest_tile(0), state{{0, 0, 0, 0}, 
                                                                 {0, 0, 0, 0}, 
                                                                 {0, 0, 0, 0}, 
                                                                 {0, 0, 0, 0}} {
            state = AddNew(state);
            state = AddNew(state);
        }
        bool CanContinue(board m);
        std::vector<int> PossibleMoves(board m);
        board Up(board m, bool peek);
        board Left(board m, bool peek);
        board Right(board m, bool peek);
        board Down(board m, bool peek);
        friend std::ostream& operator<<(std::ostream &stream, Game &game);
};