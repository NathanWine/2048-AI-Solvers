#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <random>
#include <vector>

class Game {
    protected:
        std::vector<std::vector<int>> AddNew(std::vector<std::vector<int>> m);
        std::vector<std::vector<int>> Transpose(std::vector<std::vector<int>> m);
        std::vector<std::vector<int>> MergeUp(std::vector<std::vector<int>> m);
        std::vector<std::vector<int>> CompressUp(std::vector<std::vector<int>> m);
        std::vector<std::vector<int>> HamburgerFlip(std::vector<std::vector<int>> m);
    public: 
        std::vector<std::vector<int>> state;
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
        bool CanContinue(std::vector<std::vector<int>> m);
        int* PossibleMoves();
        std::vector<std::vector<int>> Up();
        std::vector<std::vector<int>> Left();
        std::vector<std::vector<int>> Right();
        std::vector<std::vector<int>> Down();
};

std::vector<std::vector<int>> Game::AddNew(std::vector<std::vector<int>> m) {
    // Initialize rng device with random seed
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> pos_dist(0, DIM-1);

    // Generating a random position
    // TODO: FIND A BETTER METHOD OF GETTING A RANDOM EMPTY TILE
    int x = pos_dist(rng);
    int y = pos_dist(rng);
    while(m[x][y] != 0) {
        // std::cout << "Need to find a new position" << std::endl;
        x = pos_dist(rng);
        y = pos_dist(rng);
    }

    // Generating a random value of 2 or 4 (weighted probability)
    std::uniform_int_distribution<> val_dist(0, 9);
    if (val_dist(rng) > 0) {
        m[x][y] = 2;
    }
    else {
        m[x][y] = 4;
    }
    return m;
}

bool Game::CanContinue(std::vector<std::vector<int>> m) {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (m[i][j] == 0)
                return true;
            if ((i < (DIM-1) && m[i][j] == m[i + 1][j]) || 
                (j < (DIM-1) && m[i][j] == m[i][j + 1]))
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
    // Transpose();

    return move_list;
}


std::vector<std::vector<int>> Game::MergeUp(std::vector<std::vector<int>> m) {
    std::vector<std::vector<int>> state_cpy = m;
    for (int col = 0; col < DIM; col++) {
        std::vector<int> merged_column;
        for (int i = 0; i < DIM; i++) {
            if (m[i][col] != 0) {
                merged_column.push_back(m[i][col]);
            }            
        }
        while (merged_column.size() < DIM) {
            merged_column.push_back(0);
        }
        for (int i = 0; i < DIM; i++) {
            state_cpy[i][col] = merged_column[i];
        }
    }
    return state_cpy;
}

std::vector<std::vector<int>> Game::CompressUp(std::vector<std::vector<int>> m) {
    std::vector<std::vector<int>> state_cpy = m;
    for (int col = 0; col < DIM; col++) {
        std::vector<int> compressed_column;
        if (m[0][col] == m[1][col]) {
            compressed_column.push_back(m[0][col] * 2);
            score += m[0][col] * 2;
            if (m[2][col] == m[3][col]) {
                compressed_column.push_back(m[2][col] * 2);
                score += m[2][col] * 2;
            }
            else {
                compressed_column.push_back(m[2][col]);
                compressed_column.push_back(m[3][col]);
            }
        }
        else if (m[1][col] == m[2][col]) {
            compressed_column.push_back(m[0][col]);
            compressed_column.push_back(m[1][col] * 2);
            compressed_column.push_back(m[3][col]);
            score += m[1][col] * 2;
        }
        else if (m[2][col] == m[3][col]) {
            compressed_column.push_back(m[0][col]);
            compressed_column.push_back(m[1][col]);
            compressed_column.push_back(m[2][col] * 2);
            score += m[2][col] * 2;
        }
        else {
            compressed_column.push_back(m[0][col]);
            compressed_column.push_back(m[1][col]);
            compressed_column.push_back(m[2][col]);
            compressed_column.push_back(m[3][col]);
        }
        while(compressed_column.size() < DIM) {
            compressed_column.push_back(0);
        }
        for (int i = 0; i < DIM; i++) {
            state_cpy[i][col] = compressed_column[i];
        }
    }
    return state_cpy;
}

std::vector<std::vector<int>> Game::HamburgerFlip(std::vector<std::vector<int>> m) {
    std::vector<std::vector<int>> flipped_state = {{}, {}, {}, {}};
    for (int row = (DIM-1), i = 0; row >= 0; row--, i++) {
        flipped_state[i] = m[row];
    }
    return flipped_state;
}

std::vector<std::vector<int>> Game::Transpose(std::vector<std::vector<int>> m) {
    std::vector<std::vector<int>> transposed_state = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            transposed_state[i][j] = m[j][i];
        }
    }
    return transposed_state;
}

std::vector<std::vector<int>> Game::Up() {
    std::vector<std::vector<int>> prev_state = state;
    prev_state = MergeUp(prev_state);
    prev_state = CompressUp(prev_state);
    if (state != prev_state) {
        prev_state = AddNew(prev_state);
    }
    return prev_state;
}

std::vector<std::vector<int>> Game::Down() {
    std::vector<std::vector<int>> prev_state = state;
    prev_state = HamburgerFlip(prev_state);
    prev_state = Up();
    prev_state = HamburgerFlip(prev_state);
    return prev_state;
}

std::vector<std::vector<int>> Game::Left() {
    std::vector<std::vector<int>> prev_state = state;
    prev_state = Transpose(prev_state);
    prev_state = Up();
    prev_state = Transpose(prev_state);
    return prev_state;
}

std::vector<std::vector<int>> Game::Right() {
    std::vector<std::vector<int>> prev_state = state;
    prev_state = Transpose(prev_state);
    prev_state = Down();
    prev_state = Transpose(prev_state);
    return prev_state;
}

std::ostream& operator<<(std::ostream &stream, Game &game) {
    std::string str = "";
    for (int i = 0; i < game.DIM; i++) {
        for (int j = 0; j < game.DIM; j++) {
            str += std::to_string(game.state[i][j]) + " ";
        }
        str += "\n";
    }
    return stream << "Score: " + std::to_string(game.score) << std::endl << str;
}

int main(int argc, char** argv) {
    Game game = Game();
    std::cout << game;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> move_dist(0, 3);
    while(game.CanContinue(game.state)) {
        int random_number = move_dist(rng);
        if (random_number == 0) {
            game.state = game.Up();
        }
        else if (random_number == 1) {
            game.state = game.Down();
        }
        else if (random_number == 2) {
            game.state = game.Right();
        }
        else {
            game.state = game.Left();
        }
        std::cout << game << std::endl;
    }

    return 0;
}