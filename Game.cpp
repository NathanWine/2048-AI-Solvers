#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <random>

// using namespace std;

class Game {
    private:
        void AddNew();
    public: 
        int state[4][4];
        int score;
        int highest_tile;

        Game() : state{}, score(0), highest_tile(0) { }
        void Funi();
        bool CanContinue();
        int* PossibleMoves();
};

void Game::AddNew() {
    // Initialize rng device with random seed
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> pos_dist(0, 3);

    // Generating a random position
    // TODO: FIND A BETTER METHOD OF GETTING A RANDOM EMPTY TILE
    int x = pos_dist(rng);
    int y = pos_dist(rng);
    while(state[x][y] != 0) {
        std::cout << "Need to find a new position";
        x = pos_dist(rng);
        y = pos_dist(rng);
    }

    // Generating a random value of 2 or 4 (weighted probability)
    std::uniform_int_distribution<> val_dist(0, 9);
    if (val_dist(rng) > 0) {
        state[x][y] = 2;
    }
    else {
        state[x][y] = 4;
    }
}

bool Game::CanContinue() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (state[i][j] == 0)
                return true;
            if ((i < 3 && state[i][j] == state[i + 1][j]) || 
                (j < 3 && state[i][j] == state[i][j + 1]))
                return true;
        }
    }
    return false;
}

int* Game::PossibleMoves() {
    // if (!CanContinue())
    //     return [-1];
    
    int* move_list = {};

    // left_copy = 

    return move_list;
}

std::ostream& operator<<(std::ostream &stream, Game &game) {
    std::string str = "";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            str += std::to_string(game.state[i][j]) + " ";
        }
        str += "\n";
    }
    return stream << "Score: " + std::to_string(game.score) << std::endl << str;
}

// void myfun(mt19937 arengee, uniform_int_distribution<> disstrib) {
//     std::cout << disstrib(arengee) << std::endl;
// }

int main(int argc, char** argv) {
    Game game = Game();

    // std::cout << game;
    // game.CanContinue();

    std::cout << game.PossibleMoves();

    return 0;
}