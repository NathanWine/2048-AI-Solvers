#include "Game.hpp"

int main(int argc, char** argv) {
    Game game = Game();
    std::cout << game << std::endl;

    std::random_device rd;
    std::mt19937 rng(rd());
    for(std::vector<int> moves=game.possibleMoves(); 
                moves.size() > 0; moves=game.possibleMoves()) {
        // std::cout << game;
        std::uniform_int_distribution<> move_dist(0, moves.size());
        int random_number = moves[move_dist(rng)];
        for (int i = 0; i < moves.size(); i++) {
        }
        if (random_number == UP) {
            game.up(false);
        }
        else if (random_number == DOWN) {
            game.down(false);
        }
        else if (random_number == LEFT) {
            game.left(false);
        }
        else {
            game.right(false);
        }
        // std::cout << std::endl;
    }
    std::cout << game << std::endl;

    return 0;
}