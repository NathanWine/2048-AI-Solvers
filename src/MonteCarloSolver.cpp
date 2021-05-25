#include "MonteCarloSolver.hpp"

std::pair<int, int> simulateOneRun(Game game) {
    Game game_cpy = game;
    int first_move = -1;

    while(game_cpy.canContinue()) {
        board before_state = game_cpy.state;
        std::vector<int> move_bank = {UP, DOWN, LEFT, RIGHT};

        while (before_state == game_cpy.state) {
            int random_pos = DISTS[move_bank.size() - 1](rng);
            int chosen_move = move_bank[random_pos];

            if (chosen_move == UP)
                game_cpy.up(false);
            else if (chosen_move == DOWN)
                game_cpy.down(false);
            else if (chosen_move == LEFT)
                game_cpy.left(false);
            else
                game_cpy.right(false);
            if (first_move == -1)
                first_move = chosen_move;

            move_bank.erase(move_bank.begin() + random_pos);
        }
    }

    std::pair<int, int> ret(first_move, game_cpy.score);
    return ret;
}

std::pair<int, int> monteCarloSimulateGame(int runs) {
    Game game = Game();
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

        if (best_move == UP) {
            game.up(false);
        }
        else if (best_move == DOWN) {
            game.down(false);
        }
        else if (best_move == LEFT) {
            game.left(false);
        }
        else {
            game.right(false);
        }
        std::cout << game << std::endl;
    }

    std::cout << game << std::endl;
    std::pair<int, int> ret(game.getHighestTile(), game.score);
    return ret;
}

void monteCarloSolve(int n, int runs, int win=2048) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    int successes = 0;
    int attempts = 0;
    std::vector<int> scores;
    std::vector<int> highest_tiles;

    for (int i = 0; i < n; ++i) {
        attempts++;
        std::pair<int, int> result = monteCarloSimulateGame(runs);
        if (result.first >= win) {
            successes++;
        }
        highest_tiles.push_back(result.first);
        scores.push_back(result.second);
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Success rate: " << (float) successes / attempts * 100 << "%" << std::endl;
    std::cout << "Duration: " << time_span.count() << " seconds" << std::endl;
}
