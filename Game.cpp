#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <random>
#include <vector>

class Game {
    private:
    public: 
        std::vector<std::vector<int>> state;
        int score;
        int highest_tile;

        Game() : state{{0, 0, 0, 0}, 
                       {0, 0, 0, 0}, 
                       {0, 0, 0, 0}, 
                       {0, 0, 0, 0}}, score(0), highest_tile(0) { }
        bool CanContinue();
        int* PossibleMoves();
        void Transpose();
        void AddNew();
        void MergeUp();
        void CompressUp();
        void HamburgerFlip();
        void Up();
        void Left();
        void Right();
        void Down();
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
        // std::cout << "Need to find a new position" << std::endl;
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


void Game::MergeUp() {
    for (int col = 0; col < 4; col++) {
        std::vector<int> merged_column;
        for (int i = 0; i < 4; i++) {
            if (state[i][col] != 0) {
                merged_column.push_back(state[i][col]);
            }            
        }
        while (merged_column.size() < 4) {
            merged_column.push_back(0);
        }
        for (int i = 0; i < 4; i++) {
            state[i][col] = merged_column[i];
        }
    }
}

void Game::CompressUp() {
    for (int col = 0; col < 4; col++) {
        std::vector<int> compressed_column;
        if (state[0][col] == state[1][col]) {
            compressed_column.push_back(state[0][col] * 2);
            score += state[0][col] * 2;
            if (state[2][col] == state[3][col]) {
                compressed_column.push_back(state[2][col] * 2);
                compressed_column.push_back(0);
                compressed_column.push_back(0);
                score += state[2][col] * 2;
            }
            else {
                compressed_column.push_back(state[2][col]);
                compressed_column.push_back(state[3][col]);
                compressed_column.push_back(0);
            }
        }
        else if (state[1][col] == state[2][col]) {
            compressed_column.push_back(state[0][col]);
            compressed_column.push_back(state[1][col] * 2);
            compressed_column.push_back(state[3][col]);
            compressed_column.push_back(0);
            score += state[1][col] * 2;
        }
        else if (state[2][col] == state[3][col]) {
            compressed_column.push_back(state[0][col]);
            compressed_column.push_back(state[1][col]);
            compressed_column.push_back(state[2][col] * 2);
            compressed_column.push_back(0);
            score += state[2][col] * 2;
        }
        else {
            compressed_column.push_back(state[0][col]);
            compressed_column.push_back(state[1][col]);
            compressed_column.push_back(state[2][col]);
            compressed_column.push_back(state[3][col]);
        }
        for (int i = 0; i < 4; i++) {
            state[i][col] = compressed_column[i];
        }
    }
}

void Game::HamburgerFlip() {
    std::vector<std::vector<int>> flipped_state = {{}, {}, {}, {}};
    for (int row = 3, i = 0; row >= 0; row--, i++) {
        flipped_state[i] = state[row];
    }
    state = flipped_state;
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

void Game::Up() {
    std::vector<std::vector<int>> prev_state = state;
    MergeUp();
    CompressUp();
    if (state != prev_state) {
        AddNew();
    }
}

void Game::Down() {
    HamburgerFlip();
    Up();
    HamburgerFlip();
}

void Game::Left() {
    Transpose();
    Up();
    Transpose();
}

void Game::Right() {
    Transpose();
    Down();
    Transpose();
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
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> move_dist(0, 3);
    Game game = Game();

    game.AddNew();
    game.AddNew();

    std::cout << game;
    // game.HamburgerFlip();
    // std::cout << game;

    // std::vector<int> v1 = {0, 1, 2, 3};
    // std::vector<int> v2 = {0, 1, 2, 3};
    // std::cout << (v1 == v2) << std::endl;
    while(game.CanContinue()) {
        int random_number = move_dist(rng);
        if (random_number == 0) {
            game.Up();
        }
        else if (random_number == 1) {
            game.Down();
        }
        else if (random_number == 2) {
            game.Right();
        }
        else {
            game.Left();
        }
        std::cout << game;
    }

    return 0;
}