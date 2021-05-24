#include "Game.hpp"
// #include "MonteCarloSolver.hpp"

/**
 * Function to print 2D vectors (for debugging purposes)
 */
void print_vec(board v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            std::cout << v[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Game::AddNew() {
    // Create vector of empty tiles
    std::vector<std::pair<int, int>> empty_tiles;
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (state[i][j] == 0) {
                empty_tiles.push_back(std::pair<int, int>(i, j));
            }
        }
    }

    int size = empty_tiles.size();
    if (size > 0) {
        // Initialize rng device with random seed
        std::random_device rd;
        std::mt19937 rng(rd());

        // Get random empty tile coordinates
        std::uniform_int_distribution<> pos_dist(0, size - 1);
        std::pair<int, int> choice = empty_tiles[pos_dist(rng)];

        // Generating a random value of 2 or 4 (weighted probability)
        std::uniform_int_distribution<> val_dist(0, 9);
        if (val_dist(rng) > 0) {
            state[choice.first][choice.second] = 2;
        }
        else {
            state[choice.first][choice.second] = 4;
        }
    }
}

bool Game::CanContinue() {
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (state[i][j] == 0)
                return true;
            if ((i < (DIM-1) && state[i][j] == state[i + 1][j]) || 
                (j < (DIM-1) && state[i][j] == state[i][j + 1]))
                return true;
        }
    }
    return false;
}

std::vector<int> Game::PossibleMoves() {
    std::vector<int> move_list;
    if (CanContinue()) {
        Game up_copy = *this;
        up_copy.Up(true);
        if (up_copy.state != state) {
            move_list.push_back(UP);
        }

        Game down_copy = *this;
        down_copy.Down(true);
        if (down_copy.state != state) {
            move_list.push_back(DOWN);
        }

        Game left_copy = *this;
        left_copy.Left(true);
        if (left_copy.state != state) {
            move_list.push_back(LEFT);
        }

        Game right_copy = *this;
        right_copy.Right(true);
        if (right_copy.state != state) {
            move_list.push_back(RIGHT);
        }
    }
    return move_list;
}

void Game::MergeUp() {
    board state_cpy = state;
    for (int col = 0; col < DIM; col++) {
        std::vector<int> merged_column;
        for (int i = 0; i < DIM; i++) {
            if (state[i][col] != 0) {
                merged_column.push_back(state[i][col]);
            }            
        }
        while (merged_column.size() < DIM) {
            merged_column.push_back(0);
        }
        for (int i = 0; i < DIM; i++) {
            state_cpy[i][col] = merged_column[i];
        }
    }
    state = state_cpy;
}

void Game::CompressUp(bool peak) {
    board state_cpy = state;
    for (int col = 0; col < DIM; col++) {
        std::vector<int> compressed_column;
        if (state[0][col] == state[1][col]) {
            compressed_column.push_back(state[0][col] * 2);
            if (!peak) {
                score += state[0][col] * 2;
                if (state[0][col] * 2 > highest_tile) {
                    highest_tile = state[0][col];
                }
            }
            if (state[2][col] == state[3][col]) {
                compressed_column.push_back(state[2][col] * 2);
                if (!peak) {
                    score += state[2][col] * 2;
                    if (state[0][col] * 2 > highest_tile) {
                        highest_tile = state[2][col];
                    }
                }
            }
            else {
                compressed_column.push_back(state[2][col]);
                compressed_column.push_back(state[3][col]);
            }
        }
        else if (state[1][col] == state[2][col]) {
            compressed_column.push_back(state[0][col]);
            compressed_column.push_back(state[1][col] * 2);
            compressed_column.push_back(state[3][col]);
            if (!peak) {
                score += state[1][col] * 2;
                if (state[0][col] * 2 > highest_tile) {
                    highest_tile = state[1][col];
                }
            }
        }
        else if (state[2][col] == state[3][col]) {
            compressed_column.push_back(state[0][col]);
            compressed_column.push_back(state[1][col]);
            compressed_column.push_back(state[2][col] * 2);
            if (!peak) {
                score += state[2][col] * 2;
                if (state[0][col] * 2 > highest_tile) {
                    highest_tile = state[2][col];
                }
            }
        }
        else {
            compressed_column.push_back(state[0][col]);
            compressed_column.push_back(state[1][col]);
            compressed_column.push_back(state[2][col]);
            compressed_column.push_back(state[3][col]);
        }
        while(compressed_column.size() < DIM) {
            compressed_column.push_back(0);
        }
        for (int i = 0; i < DIM; i++) {
            state_cpy[i][col] = compressed_column[i];
        }
    }
    state = state_cpy;
}

void Game::HamburgerFlip() {
    board flipped_state = {{}, {}, {}, {}};
    for (int row = (DIM-1), i = 0; row >= 0; row--, i++) {
        flipped_state[i] = state[row];
    }
    state = flipped_state;
}

void Game::Transpose() {
    board transposed_state = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            transposed_state[i][j] = state[j][i];
        }
    }
    state = transposed_state;
}

void Game::Up(bool peak) {
    board prev_state = state;
    MergeUp();
    CompressUp(peak);
    if (state != prev_state) {
        AddNew();
    }
}

void Game::Down(bool peak) {
    HamburgerFlip();
    Up(peak);
    HamburgerFlip();
}

void Game::Left(bool peak) {
    Transpose();
    Up(peak);
    Transpose();
}

void Game::Right(bool peak) {
    Transpose();
    Down(peak);
    Transpose();
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
