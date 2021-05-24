#include "Game.hpp"

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

/**
 * Binary search function for determining the number of digits in a number
 * (more efficient than the log method) 
 * from: https://stackoverflow.com/questions/1489830/efficient-way-to-determine-number-of-digits-in-an-integer
 */
int numDigits(int32_t x)
{
    if (x >= 10000) {
        if (x >= 10000000) {
            if (x >= 100000000) {
                if (x >= 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if (x >= 100000) {
            if (x >= 1000000)
                return 7;
            return 6;
        }
        return 5;
    }
    if (x >= 100) {
        if (x >= 1000)
            return 4;
        return 3;
    }
    if (x >= 10)
        return 2;
    return 1;
}

void Game::addNew() {
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

bool Game::canContinue() {
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

std::vector<int> Game::possibleMoves() {
    std::vector<int> move_list;
    if (canContinue()) {
        Game up_copy = *this;
        up_copy.up(true);
        if (up_copy.state != state) {
            move_list.push_back(UP);
        }

        Game down_copy = *this;
        down_copy.down(true);
        if (down_copy.state != state) {
            move_list.push_back(DOWN);
        }

        Game left_copy = *this;
        left_copy.left(true);
        if (left_copy.state != state) {
            move_list.push_back(LEFT);
        }

        Game right_copy = *this;
        right_copy.right(true);
        if (right_copy.state != state) {
            move_list.push_back(RIGHT);
        }
    }
    return move_list;
}

void Game::mergeUp() {
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

void Game::compressUp(bool peak) {
    board state_cpy = state;
    for (int col = 0; col < DIM; col++) {
        std::vector<int> compressed_column;
        if (state[0][col] == state[1][col]) {
            compressed_column.push_back(state[0][col] * 2);
            if (!peak) {
                score += state[0][col] * 2;
            }
            if (state[2][col] == state[3][col]) {
                compressed_column.push_back(state[2][col] * 2);
                if (!peak) {
                    score += state[2][col] * 2;
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
            }
        }
        else if (state[2][col] == state[3][col]) {
            compressed_column.push_back(state[0][col]);
            compressed_column.push_back(state[1][col]);
            compressed_column.push_back(state[2][col] * 2);
            if (!peak) {
                score += state[2][col] * 2;
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

void Game::hamburgerFlip() {
    board flipped_state = {{}, {}, {}, {}};
    for (int row = (DIM-1), i = 0; row >= 0; row--, i++) {
        flipped_state[i] = state[row];
    }
    state = flipped_state;
}

void Game::transpose() {
    board transposed_state = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            transposed_state[i][j] = state[j][i];
        }
    }
    state = transposed_state;
}

void Game::up(bool peak) {
    board prev_state = state;
    mergeUp();
    compressUp(peak);
    if (state != prev_state) {
        addNew();
    }
}

void Game::down(bool peak) {
    hamburgerFlip();
    up(peak);
    hamburgerFlip();
}

void Game::left(bool peak) {
    transpose();
    up(peak);
    transpose();
}

void Game::right(bool peak) {
    transpose();
    down(peak);
    transpose();
}

int Game::getHighestTile() {
    int highest_tile = 2;
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (state[i][j] > highest_tile) {
                highest_tile = state[i][j];
            }
        }
    }
    return highest_tile;
}

std::ostream& operator<<(std::ostream &stream, Game &game) {
    int max_len = numDigits(game.getHighestTile());
    std::string str = "";
    for (int i = 0; i < game.DIM; i++) {
        for (int j = 0; j < game.DIM; j++) {
            int number_of_spaces = max_len - numDigits(game.state[i][j]) + 1;
            str += std::to_string(game.state[i][j]);
            for (int k = 0; k < number_of_spaces; k++) {
                str += " ";
            }
        }
        str += "\n";
    }
    return stream << "Score: " + std::to_string(game.score) << std::endl << str;
}
