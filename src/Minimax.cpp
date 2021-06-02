#include "Minimax.hpp"

float minimaxScore(int depth, Game game) {
    float score = 0.0;
    for (int stage = 0; stage < depth; ++stage) {
        Game best_game;
        float best_score = -std::numeric_limits<float>::max();

        // CHECK FOR CANCONTINUE?
        movelist possible_moves = game.possibleMoves();
        if ((int) possible_moves.size() == 0) {
            std::cout << std::endl << "**********" << std::endl << "I don't think this condition is ever truuuuuueeeee" << std::endl << "***********" << std::endl << std::endl;
            return 0.0;
        }
        
        for (int i = 0; i < (int) possible_moves.size(); i++) {
            // Game game_copy = game;
            // int move = possible_moves[i].first;
            // if (move == UP) {
            //     game_copy.up(true);
            // }
            // else if (move == DOWN) {
            //     game_copy.down(true);
            // }
            // else if (move == LEFT) {
            //     game_copy.left(true);
            // }
            // else if (move == RIGHT) {
            //     game_copy.right(true);
            // }

            // if (get_score(game_copy) >= best_score) {
            //     best_game = game_copy;
            //     best_score = get_score(game_copy);
            // }
            float copy_h_score = get_score(possible_moves[i].second);
            if (copy_h_score > best_score) {
                best_game = possible_moves[i].second;
                best_score = copy_h_score;
            }
        }

        Game worst_game;
        float worst_score = std::numeric_limits<float>::max();
        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                if (best_game.state[i][j] == 0) {
                    Game game_copy = best_game;
                    game_copy.state[i][j] = 2;
                    float copy_h_score = get_score(game_copy);
                    if (copy_h_score < worst_score) {
                        worst_game = game_copy;
                        worst_score = copy_h_score;
                    }

                    game_copy.state[i][j] = 4;
                    copy_h_score = get_score(game_copy);
                    if (copy_h_score < worst_score) {
                        worst_game = game_copy;
                        worst_score = copy_h_score;
                    }
                }
            }
        }
        Game gameee = worst_game;
        score = get_score(gameee);
        // score = worst_score;
    }
    return score;
}

std::pair<int, int> miniMaxSearch(int depth, int display_level) {
    Game game = Game();
    std::cout << "Attempting to solve a new game with MiniMax..." << std::endl;
    if (display_level >= 1) {
        std::cout << game << std::endl;
    }
    while (game.canContinue()) {
        if (display_level >= 2) {
            std::cout << game << "Heuristic score: " << get_score(game) << std::endl;
        }
        std::map<int, weightedmoves> possibilities = game.computePossibilities();
        std::map<int, float> scores;
        std::cout << "Possiblilities size: " << possibilities.size() << std::endl;

        for (auto const& entry : possibilities) {
            int move = entry.first;
            weightedmoves weighted_subset = entry.second;
            int len = (int) weighted_subset.size();
            if (len > 0) {
                scores[move] = 0.0;
                for (int j = 0; j < len; ++j) {
                    scores[move] += minimaxScore(depth, weighted_subset[j].second) * weighted_subset[j].first;
                }
            }
        }

        std::cout << "Scores: ";
        for (auto const& score : scores) {
            std::cout << score.first << ": " << score.second << ", ";
        }
        std::cout << std::endl;

        float max_score = -std::numeric_limits<float>::max();
        int max_choice = UP;
        for (auto const& score : scores) {
            if (score.second > max_score) {
                max_score = score.second;
                max_choice = score.first;
            }
        }

        std::cout << "Max choice: " << max_choice << std::endl << std::endl;

        if (max_choice == UP)
            game.up(false);
        else if (max_choice == DOWN)
            game.down(false);
        else if (max_choice == LEFT)
            game.left(false);
        else if (max_choice == RIGHT)
            game.right(false);
    }
    if (display_level >= 1) {
        std::cout << game << std::endl;
    }
    return std::pair<int, int>(game.getHighestTile(), game.score);
}

void miniMaxSolve(int n, int depth, int display_level, int win) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    int successes = 0;
    int attempts = 0;
    std::vector<int> scores;
    std::vector<int> highest_tiles;

    for (int i = 0; i < n; ++i) {
        attempts++;
        std::pair<int, int> result = miniMaxSearch(depth, display_level);
        if (result.first >= win) {
            successes++;
        }
        highest_tiles.push_back(result.first);
        scores.push_back(result.second);
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Success rate: " << (float) successes / attempts * 100 << "%" << std::endl;
    if (n > 1) {
        float average = std::accumulate(scores.begin(), scores.end(), 0.0) / n;
        std::cout << "Average score: " << average << std::endl;
        std::cout << "Highest tiles: ";
        for (int i = 0; i < (int) highest_tiles.size(); ++i) {
            std::cout << highest_tiles[i] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "Time elapsed: " << time_span.count() << " seconds" << std::endl;
}
