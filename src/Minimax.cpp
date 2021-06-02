#include "Minimax.hpp"

/**
 * From a given game state, determines the highest scoring move to make based on heuristics 
 * and then returns the heuristic score of the worst permutation from that "best" move.
 * @return float SCORE
 */
float minimaxScore(int depth, Game game) {
    // Not sure if depth is working properly. Following the code from python codebase
    float score = 0.0;
    for (int stage = 0; stage < depth; ++stage) {
        movelist possible_moves = game.possibleMoves();
        if ((int) possible_moves.size() == 0) {
            return 0.0;
        }
        
        Game best_game;
        float best_score = -std::numeric_limits<float>::max();
        for (int i = 0; i < (int) possible_moves.size(); i++) {
            float copy_h_score = get_h_score(possible_moves[i].second);
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
                    float copy_h_score = get_h_score(game_copy);
                    if (copy_h_score < worst_score) {
                        worst_game = game_copy;
                        worst_score = copy_h_score;
                    }

                    game_copy.state[i][j] = 4;
                    copy_h_score = get_h_score(game_copy);
                    if (copy_h_score < worst_score) {
                        worst_game = game_copy;
                        worst_score = copy_h_score;
                    }
                }
            }
        }
        score = worst_score;
    }
    return score;
}

/**
 * Determines the weighted likelihoods of each possible move along with all following permutations.
 * For each possible move, scores each of its following permutations and then executes the best.
 * @return (int HIGHEST_TILE, int FINAL_SCORE)
 */
std::pair<int, int> miniMaxSearch(int depth, int display_level) {
    Game game = Game();
    std::cout << "Attempting to solve a new game with MiniMax..." << std::endl;
    if (display_level >= 1) {
        std::cout << game << std::endl;
    }
    while (game.canContinue()) {
        if (display_level >= 2) {
            std::cout << game;
        }

        std::map<int, weightedmoves> possibilities = game.computePossibilities();
        std::map<int, float> scores;

        for (auto const& entry : possibilities) {
            int move = entry.first;                             // Direction of move
            weightedmoves weighted_subset = entry.second;       // Vector of (prob, game)
            int len = (int) weighted_subset.size();
            if (len > 0) {
                scores[move] = 0.0;
                for (int j = 0; j < len; ++j) {
                    scores[move] += minimaxScore(depth, weighted_subset[j].second) * weighted_subset[j].first;
                }
            }
        }

        if (display_level >= 3) {
            std::cout << "Heuristic score: " << get_h_score(game) << std::endl << "Move scores: ";
            for (auto const& score : scores) {
                std::cout << score.first << ": " << score.second << ", ";
            }
            std::cout << std::endl;
        }

        float max_score = -std::numeric_limits<float>::max();
        int max_choice = UP;
        for (auto const& score : scores) {      // Determine best move choice based on tabulated scores
            if (score.second > max_score) {
                max_score = score.second;
                max_choice = score.first;
            }
        }

        switch (max_choice) {
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
        }

        if (display_level >= 2) {
            std::cout << std::endl;
        }
    }
    if (display_level >= 1) {
        std::cout << game << std::endl;
    }
    return std::pair<int, int>(game.getHighestTile(), game.score);
}

/**
 * Creates and completes n-many games using the MiniMax simulation function.
 * Displays cumulative stats at completion.
 */
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
