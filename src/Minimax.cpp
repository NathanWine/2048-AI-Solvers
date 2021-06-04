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
std::pair<int, int> minimaxSearch(int depth, int display_level) {
    Game game = Game();
    std::cout << "Attempting to solve a new game with Minimax..." << std::endl;
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
 * Creates and completes n-many games using the Minimax simulation function.
 * Tabulates data from game each in order to display results at completion.
 */
int minimaxSolve(int n, int depth, int display_level, 
    std::vector<int> *scores, std::vector<int> *highest_tiles) {

    int successes = 0;
    for (int i = 0; i < n; ++i) {
        std::pair<int, int> result = minimaxSearch(depth, display_level);
        if (result.first >= WIN) {
            successes++;
        }
        (*highest_tiles).push_back(result.first);
        (*scores).push_back(result.second);
    }
    return successes;
}
