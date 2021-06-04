#include "MonteCarlo.hpp"

/**
 * From a given game state, chooses random moves until the game is completed.
 * @return (int FIRST_MOVE, int FINAL_SCORE)
*/
std::pair<int, int> simulateOneRun(Game game) {
    Game game_cpy = game;
    int first_move = -1;

    while(game_cpy.canContinue()) {
        board before_state = game_cpy.state;
        std::vector<int> move_bank = {UP, DOWN, LEFT, RIGHT};

        while (before_state == game_cpy.state) {
            int random_pos = DISTS[move_bank.size() - 1](rng);
            int chosen_move = move_bank[random_pos];

            switch(chosen_move) {
                case UP:
                    game_cpy.up(false);
                    break;
                case DOWN:
                    game_cpy.down(false);
                    break;
                case LEFT:
                    game_cpy.left(false);
                    break;
                case RIGHT:
                    game_cpy.right(false);
                    break;
            }
            if (first_move == -1) {
                first_move = chosen_move;
            }
            move_bank.erase(move_bank.begin() + random_pos);
        }
    }

    std::pair<int, int> ret(first_move, game_cpy.score);
    return ret;
}

/**
 * Creates a new game and then completes the game from its current state with 
 * completely random moves until RUNS-many completions. The looks at the scores
 * from those random runs to decide the best move for the current state and 
 * executes that move. Continues this process until game completion.
 * @return (int HIGHEST_TILE, int FINAL_SCORE)
 */
std::pair<int, int> monteCarloSimulateGame(int runs, int display_level) {
    Game game = Game();
    std::cout << "Attempting to solve a new game with Monte Carlo..." << std::endl;
    if (display_level >= 1) {
        std::cout << game << std::endl;
    }
    while (game.canContinue()) {
        int scores[4] = {0, 0, 0, 0};
        int counter[4] = {0, 0, 0, 0};

        for (int i = 0; i < runs; ++i) {
            std::pair<int, int> res = simulateOneRun(game);
            scores[res.first] += res.second;
            counter[res.first] += 1;
        }

        int best_avg_score = 0;
        int best_move = UP;
        for (int i = 0; i < 4; ++i) {
            int move = i;
            if (counter[move] != 0 && ((float) scores[move] / counter[move]) > best_avg_score) {
                best_avg_score = (float) scores[move] / counter[move];
                best_move = move;
            }
        }

        switch(best_move) {
            case UP:
                game.up(false);
                break;
            case DOWN:
                game.down(false);
                break;
            case LEFT:
                game.left(false);
                break;
            case RIGHT:
                game.right(false);
                break;
        }
        if (display_level >= 2) {
            std::cout << game << std::endl;
        }
    }
    if (display_level >= 1) {
        std::cout << game << std::endl;
    }
    std::pair<int, int> ret(game.getHighestTile(), game.score);
    return ret;
}

/**
 * Creates and completes n-many games using the MonteCarlo simulation function.
 * Tabulates data from each game in order to display results at completion.
 */
int monteCarloSolve(int n, int runs, int display_level,
    std::vector<int> *scores, std::vector<int> *highest_tiles) {

    int successes = 0;
    for (int i = 0; i < n; ++i) {
        std::pair<int, int> result = monteCarloSimulateGame(runs, display_level);
        if (result.first >= WIN) {
            successes++;
        }
        (*highest_tiles).push_back(result.first);
        (*scores).push_back(result.second);
    }
    return successes;
}
