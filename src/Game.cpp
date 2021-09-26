#include "Game.hpp"
#include "Util.hpp"

// Initialize rnd devices and distributions
std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution<> DISTS[] = {             // Really ugly...
    std::uniform_int_distribution<>(0, 0), std::uniform_int_distribution<>(0, 1), 
    std::uniform_int_distribution<>(0, 2), std::uniform_int_distribution<>(0, 3),
    std::uniform_int_distribution<>(0, 4), std::uniform_int_distribution<>(0, 5), 
    std::uniform_int_distribution<>(0, 6), std::uniform_int_distribution<>(0, 7),
    std::uniform_int_distribution<>(0, 8), std::uniform_int_distribution<>(0, 9), 
    std::uniform_int_distribution<>(0, 10), std::uniform_int_distribution<>(0, 11),
    std::uniform_int_distribution<>(0, 12), std::uniform_int_distribution<>(0, 13), 
    std::uniform_int_distribution<>(0, 13), std::uniform_int_distribution<>(0, 15),
};

void Game::addNew() {
    // Create vector of empty tiles
    std::vector<std::pair<int, int>> empty_tiles;
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            if (state[i][j] == 0) {
                empty_tiles.push_back(std::pair<int, int>(i, j));
            }
        }
    }

    int size = empty_tiles.size();
    if (size > 0) {
        // Get random empty tile coordinates
        std::pair<int, int> choice = empty_tiles[DISTS[size-1](rng)];

        // Generating a random value of 2 or 4 (weighted probability)
        if (DISTS[9](rng) > 0) {
            state[choice.first][choice.second] = 2;
        }
        else {
            state[choice.first][choice.second] = 4;
        }
    }
}

bool Game::canContinue() const {
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            if (state[i][j] == 0)
                return true;
            if ((i < (DIM-1) && state[i][j] == state[i + 1][j]) || 
                (j < (DIM-1) && state[i][j] == state[i][j + 1]))    // If adj. hor/ver match
                return true;
        }
    }
    return false;
}

/**
 * Creates and returns a vector of each move that can be made given the current state, along 
 * with a copy of the game if that move were to be made (WITHOUT RANDOM NEW TILE ADDITIONS)
 * @return vector of (int MOVE, Game GAME_IF_MOVE_EXECUTED)
 */
bool Game::generatePossibleMoves(movelist &move_list) const {
    // Note: Responsibility to check if game can continue is given to other functions
    Game up_copy = *this;
    up_copy.up(true);
    if (up_copy.state != state) {
        move_list.push_back(std::pair<int, Game>(UP, up_copy));
    }

    Game down_copy = *this;
    down_copy.down(true);
    if (down_copy.state != state) {
        move_list.push_back(std::pair<int, Game>(DOWN, down_copy));
    }

    Game left_copy = *this;
    left_copy.left(true);
    if (left_copy.state != state) {
        move_list.push_back(std::pair<int, Game>(LEFT, left_copy));
    }

    Game right_copy = *this;
    right_copy.right(true);
    if (right_copy.state != state) {
        move_list.push_back(std::pair<int, Game>(RIGHT, right_copy));
    }
    return move_list.size() > 0;
}

/**
 * Determines and returns the pool of valid moves at a given game state mapped to the 
 * probability and following game state of ALL permutations from EACH of those moves.
 * @return map of [MOVE, vector of (float PROBABILITY, Game POTENTIAL_GAME_STATE)]
 */
std::map<int, weightedmoves> Game::computePossibilities() const {
    // Note: Responsibility to check if game can continue is given to other functions
    std::map<int, weightedmoves> possibilities;
    movelist valid_moves;
    generatePossibleMoves(valid_moves);

    for (auto const &move : valid_moves) {
        int dir = move.first;
        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                if (move.second.state[i][j] == 0) {      // If space is 0, add all outcomes if tile added here
                    Game game_copy = move.second;
                    game_copy.state[i][j] = 2;
                    possibilities[dir].push_back(std::pair<float, Game>(0.9, game_copy));
                    game_copy.state[i][j] = 4;
                    possibilities[dir].push_back(std::pair<float, Game>(0.1, game_copy));
                }
            }
        }
    }

    for (auto &possibility : possibilities) {           // Normalize the probabilities
        int len = (int) possibility.second.size();
        for (int i = 0; i < len; ++i) {
            possibility.second[i].first /= len;
        }
    }

    return possibilities;
}

void Game::mergeUp() {
    for (int col = 0; col < DIM; ++col) {
        std::vector<int> merged_column;
        for (int i = 0; i < DIM; ++i) {
            if (state[i][col] != 0) {
                merged_column.push_back(state[i][col]);
            }            
        }
        while (merged_column.size() < DIM) {
            merged_column.push_back(0);
        }
        for (int i = 0; i < DIM; ++i) {
            state[i][col] = merged_column[i];
        }
    }
}

void Game::compressUp(bool peak) {
    for (int col = 0; col < DIM; ++col) {
        std::vector<int> compressed_column;
        if (state[0][col] == state[1][col]) {
            compressed_column.push_back(state[0][col] * 2);
            score += state[0][col] * 2;
            if (state[2][col] == state[3][col]) {
                compressed_column.push_back(state[2][col] * 2);
                score += state[2][col] * 2;
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
            score += state[1][col] * 2;
        }
        else if (state[2][col] == state[3][col]) {
            compressed_column.push_back(state[0][col]);
            compressed_column.push_back(state[1][col]);
            compressed_column.push_back(state[2][col] * 2);
            score += state[2][col] * 2;
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
        for (int i = 0; i < DIM; ++i) {
            state[i][col] = compressed_column[i];
        }
    }
}

void Game::hamburgerFlip() {
    board flipped_state = {{}, {}, {}, {}};
    for (int row = (DIM-1), i = 0; row >= 0; --row, ++i) {
        flipped_state[i] = state[row];
    }
    state = flipped_state;
}

void Game::transpose() {
    board transposed_state = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            transposed_state[i][j] = state[j][i];
        }
    }
    state = transposed_state;
}

void Game::up(bool peak) {
    board prev_state = state;
    mergeUp();
    compressUp(peak);
    if (state != prev_state && !peak) {
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

int Game::getHighestTile() const {
    int highest_tile = 2;
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            if (state[i][j] > highest_tile) {
                highest_tile = state[i][j];
            }
        }
    }
    return highest_tile;
}

int Game::getNumberEmpty(const board &game_state) {
    int sum = 0;
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            if (game_state[i][j] == 0) {
                sum += 1;
            }
        }
    }
    return sum;
}

const std::string Game::printBoard(int *width = NULL) const {
    int max_len = Util::numDigits(getHighestTile());
    std::string str = "";
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            int number_of_spaces = max_len - Util::numDigits(state[i][j]) + 1;
            str += std::to_string(state[i][j]);
            for (int k = 0; k < number_of_spaces; ++k) {
                str += " ";
            }
        }
        if (i < DIM - 1) {
            str += "\n";
        }
    }
    if (width != NULL) {
        *width = (max_len * DIM) + DIM;
    }
    return str;
}

const std::string Game::staticPrint(const Game &game, int *width) {
    return game.printBoard(width);
}

std::ostream& operator<<(std::ostream &stream, const Game &game) {
    return stream << "Score: " + std::to_string(game.score) << "\n" << game.printBoard() << "\n";
}
