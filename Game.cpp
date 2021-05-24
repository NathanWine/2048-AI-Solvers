// #include <iostream>
// #include <string>
// #include <ctime>
// #include <cstdlib>
// #include <random>
// #include <vector>
// #include <algorithm>
#include "Game.hpp"
// #include "MonteCarloSolver.hpp"

// enum MOVES {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3};
// typedef std::vector<std::vector<int>> board;

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

// class Game {
//     protected:
//         board AddNew(board m);
//         board Transpose(board m);
//         board MergeUp(board m);
//         board CompressUp(board m, bool peek);
//         board HamburgerFlip(board m);
//     public: 
//         board state;
//         int score;
//         int highest_tile;
//         const int DIM;

//         Game(int d=4) : DIM(d), score(0), highest_tile(0), state{{0, 0, 0, 0}, 
//                                                                  {0, 0, 0, 0}, 
//                                                                  {0, 0, 0, 0}, 
//                                                                  {0, 0, 0, 0}} {
//             state = AddNew(state);
//             state = AddNew(state);
//         }
//         bool CanContinue(board m);
//         std::vector<int> PossibleMoves(board m);
//         board Up(board m, bool peek);
//         board Left(board m, bool peek);
//         board Right(board m, bool peek);
//         board Down(board m, bool peek);
// };

board Game::AddNew(board m) {
    // Create vector of empty tiles
    std::vector<std::pair<int, int>> empty_tiles;
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (m[i][j] == 0) {
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
            m[choice.first][choice.second] = 2;
        }
        else {
            m[choice.first][choice.second] = 4;
        }
    }

    return m;
}

bool Game::CanContinue(board m) {
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

std::vector<int> Game::PossibleMoves(board m) {
    std::vector<int> move_list;
    if (CanContinue(m)) {
        board up_copy = Up(m, true);
        if (up_copy != m) {
            move_list.push_back(UP);
        }

        board down_copy = Down(m, true);
        if (down_copy != m) {
            move_list.push_back(DOWN);
        }

        board left_copy = Left(m, true);
        if (left_copy != m) {
            move_list.push_back(LEFT);
        }

        board right_copy = Right(m, true);
        if (right_copy != m) {
            move_list.push_back(RIGHT);
        }
    }
    return move_list;
}

board Game::MergeUp(board m) {
    board state_cpy = m;
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

board Game::CompressUp(board m, bool peak=false) {
    board state_cpy = m;
    for (int col = 0; col < DIM; col++) {
        std::vector<int> compressed_column;
        if (m[0][col] == m[1][col]) {
            compressed_column.push_back(m[0][col] * 2);
            if (!peak) {
                score += m[0][col] * 2;
                if (m[0][col] * 2 > highest_tile) {
                    highest_tile = m[0][col];
                }
            }
            if (m[2][col] == m[3][col]) {
                compressed_column.push_back(m[2][col] * 2);
                if (!peak) {
                    score += m[2][col] * 2;
                    if (m[0][col] * 2 > highest_tile) {
                        highest_tile = m[2][col];
                    }
                }
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
            if (!peak) {
                score += m[1][col] * 2;
                if (m[0][col] * 2 > highest_tile) {
                    highest_tile = m[1][col];
                }
            }
        }
        else if (m[2][col] == m[3][col]) {
            compressed_column.push_back(m[0][col]);
            compressed_column.push_back(m[1][col]);
            compressed_column.push_back(m[2][col] * 2);
            if (!peak) {
                score += m[2][col] * 2;
                if (m[0][col] * 2 > highest_tile) {
                    highest_tile = m[2][col];
                }
            }
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

board Game::HamburgerFlip(board m) {
    board flipped_state = {{}, {}, {}, {}};
    for (int row = (DIM-1), i = 0; row >= 0; row--, i++) {
        flipped_state[i] = m[row];
    }
    return flipped_state;
}

board Game::Transpose(board m) {
    board transposed_state = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            transposed_state[i][j] = m[j][i];
        }
    }
    return transposed_state;
}

board Game::Up(board m, bool peak = false) {
    board prev_state = m;
    prev_state = MergeUp(prev_state);
    prev_state = CompressUp(prev_state);
    if (state != prev_state) {
        prev_state = AddNew(prev_state);
    }
    return prev_state;
}

board Game::Down(board m, bool peak = false) {
    board prev_state = m;
    prev_state = HamburgerFlip(prev_state);
    prev_state = Up(prev_state);
    prev_state = HamburgerFlip(prev_state);
    return prev_state;
}

board Game::Left(board m, bool peak = false) {
    board prev_state = m;
    prev_state = Transpose(prev_state);
    prev_state = Up(prev_state);
    prev_state = Transpose(prev_state);
    return prev_state;
}

board Game::Right(board m, bool peak = false) {
    board prev_state = m;
    prev_state = Transpose(prev_state);
    prev_state = Down(prev_state);
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
