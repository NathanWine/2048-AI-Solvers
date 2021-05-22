#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <random>
#include <vector>

// using namespace std;

class Game {
    private:
        void MergeUp();
        void CompressUp();
        void HamburgerFlip();
    public: 
        std::vector<std::vector<int>> state;
        int score;
        int highest_tile;

        Game() : state{{0, 0, 0, 0}, 
                       {0, 0, 0, 0}, 
                       {0, 0, 0, 0}, 
                       {0, 0, 0, 0}}, score(0), highest_tile(0) { }
        void Funi();
        bool CanContinue();
        int* PossibleMoves();
        void Transpose();
                void AddNew();

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
    Transpose();

    return move_list;
}

void Game::Transpose() {
    std::vector<std::vector<int>> transposed_state = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            transposed_state[i][j] = state[j][i];
        }
    }
    // TODO: MAKE THIS RETURN TRANSPOSED STATE INSTEAD FOR MORE FLEXIBILITY???
    state = transposed_state;
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

int main(int argc, char** argv) {
    Game game = Game();

    game.AddNew();
    game.AddNew();

    std::cout << game;
    game.Transpose();
    std::cout << game;

    return 0;
}