#include "Game.hpp"

int main(int argc, char** argv) {
    Game game = Game();
    std::cout << game << std::endl;

    std::random_device rd;
    std::mt19937 rng(rd());
    for(std::vector<int> moves=game.PossibleMoves(game.state); 
                moves.size() > 0; moves=game.PossibleMoves(game.state)) {
        std::cout << game;
        std::uniform_int_distribution<> move_dist(0, moves.size());
        int random_number = moves[move_dist(rng)];
        for (int i = 0; i < moves.size(); i++) {
        }
        if (random_number == UP) {
            game.state = game.Up(game.state, false);
        }
        else if (random_number == DOWN) {
            game.state = game.Down(game.state, false);
        }
        else if (random_number == LEFT) {
            game.state = game.Left(game.state, false);
        }
        else {
            game.state = game.Right(game.state, false);
        }
        std::cout << std::endl;
    }

    // std::vector<int> v = {1, 2, 3, 10};
    // int largest_value = *std::max_element(v.begin(), v.end());
    // std::cout << "largest_value is:" << largest_value << std::endl;

    return 0;
}