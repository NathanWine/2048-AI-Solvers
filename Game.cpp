#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>

enum MOVES {UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3};

/**
 * Function to print 2D vectors (for debugging purposes)
 */
void print_vec(std::vector<std::vector<int>> v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            std::cout << v[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

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
        std::vector<int> PossibleMoves(std::vector<std::vector<int>> m);
        std::vector<std::vector<int>> Up(std::vector<std::vector<int>> m);
        std::vector<std::vector<int>> Left(std::vector<std::vector<int>> m);
        std::vector<std::vector<int>> Right(std::vector<std::vector<int>> m);
        std::vector<std::vector<int>> Down(std::vector<std::vector<int>> m);
};

std::vector<std::vector<int>> Game::AddNew(std::vector<std::vector<int>> m) {
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

std::vector<int> Game::PossibleMoves(std::vector<std::vector<int>> m) {
    std::vector<int> move_list;
    if (CanContinue(m)) {
        std::vector<std::vector<int>> up_copy = Up(m);
        if (up_copy != m) {
            move_list.push_back(UP);
        }

        std::vector<std::vector<int>> down_copy = Down(m);
        if (down_copy != m) {
            move_list.push_back(DOWN);
        }

        std::vector<std::vector<int>> left_copy = Left(m);
        if (left_copy != m) {
            move_list.push_back(LEFT);
        }

        std::vector<std::vector<int>> right_copy = Right(m);
        if (right_copy != m) {
            move_list.push_back(RIGHT);
        }
    }
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

std::vector<std::vector<int>> Game::Up(std::vector<std::vector<int>> m) {
    std::vector<std::vector<int>> prev_state = m;
    prev_state = MergeUp(prev_state);
    prev_state = CompressUp(prev_state);
    if (state != prev_state) {
        prev_state = AddNew(prev_state);
    }
    return prev_state;
}

std::vector<std::vector<int>> Game::Down(std::vector<std::vector<int>> m) {
    std::vector<std::vector<int>> prev_state = m;
    prev_state = HamburgerFlip(prev_state);
    prev_state = Up(prev_state);
    prev_state = HamburgerFlip(prev_state);
    return prev_state;
}

std::vector<std::vector<int>> Game::Left(std::vector<std::vector<int>> m) {
    std::vector<std::vector<int>> prev_state = m;
    prev_state = Transpose(prev_state);
    prev_state = Up(prev_state);
    prev_state = Transpose(prev_state);
    return prev_state;
}

std::vector<std::vector<int>> Game::Right(std::vector<std::vector<int>> m) {
    std::vector<std::vector<int>> prev_state = m;
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
            game.state = game.Up(game.state);
        }
        else if (random_number == DOWN) {
            game.state = game.Down(game.state);
        }
        else if (random_number == LEFT) {
            game.state = game.Left(game.state);
        }
        else {
            game.state = game.Right(game.state);
        }
        std::cout << std::endl;
    }


    // std::vector<int> v = {1, 2, 3, 10};
    // int largest_value = *std::max_element(v.begin(), v.end());
    // std::cout << "largest_value is:" << largest_value << std::endl;

    return 0;
}