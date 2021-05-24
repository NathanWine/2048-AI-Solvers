#include <vector>
#include <iostream>
#include <random>

enum MOVES {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3};
typedef std::vector<std::vector<int>> board;

class Game {
    protected:
        void AddNew();
        void Transpose();
        void MergeUp();
        void CompressUp(bool peak);
        void HamburgerFlip();
    public: 
        board state;
        int score;
        int highest_tile;
        const int DIM;

        Game(int d=4) : DIM(d), score(0), highest_tile(0), state{{0, 0, 0, 0}, 
                                                                 {0, 0, 0, 0}, 
                                                                 {0, 0, 0, 0}, 
                                                                 {0, 0, 0, 0}} {
            AddNew();
            AddNew();
        }
        bool CanContinue();
        std::vector<int> PossibleMoves();
        void Up(bool peak);
        void Left(bool peak);
        void Right(bool peak);
        void Down(bool peak);
        friend std::ostream& operator<<(std::ostream &stream, Game &game);
};