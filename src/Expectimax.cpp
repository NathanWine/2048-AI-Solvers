#include "Expectimax.hpp"

float expectimaxScore(int depth, Game game, bool is_max) {
    std::map<int, weightedmoves> possibilities = game.computePossibilities();    
    if (possibilities.size() == 0) {
        return 0.0;
    }
    else if (is_max) {
        if (depth == 0) {
            return Heuristics::get_h_score(game.state);
        }
        else {
            std::map<int, float> scores;
            for (auto const& entry : possibilities) {
                int move = entry.first;                             // Direction of move
                weightedmoves weighted_subset = entry.second;       // Vector of (prob, game)
                int len = (int) weighted_subset.size();
                if (len > 0) {
                    scores[move] = 0.0;
                    for (int j = 0; j < len; ++j) {
                        scores[move] += Heuristics::get_h_score(weighted_subset[j].second.state) * weighted_subset[j].first;
                    }
                }
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

            return expectimaxScore(depth - 1, game, false);
        }
    }
    else {
        std::map<int, float> scores;
        for (auto const& entry : possibilities) {
            int move = entry.first;                             // Direction of move
            weightedmoves weighted_subset = entry.second;       // Vector of (prob, game)
            int len = (int) weighted_subset.size();
            if (len > 0) {
                scores[move] = 0.0;
                for (int j = 0; j < len; ++j) {
                    scores[move] += expectimaxScore(depth, weighted_subset[j].second, true) * weighted_subset[j].first;
                }
            }
        }
        float sum = 0.0;
        for (auto const& score : scores) {
            sum += score.second;
        }
        return sum / scores.size();
    }
}

std::pair<int, int> expectimaxSearch(int depth, int display_level, Game game) {
    std::cout << "Attempting to solve a new game with Expectimax... " << std::flush;
    while (game.canContinue()) {
        if (display_level >= 2) {
            std::cout << std::endl << game;
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
                    scores[move] += expectimaxScore(depth, weighted_subset[j].second, true) * weighted_subset[j].first;
                }
            }
        }
        if (display_level >= 3) {
            std::cout << "Heuristic score: " << Heuristics::get_h_score(game.state) << std::endl << "Move scores: ";
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
    }
    if (display_level <= 1) {
        std::cout << "Done!" << (display_level == 0 ? "\n" : "");
    }
    if (display_level >= 1) {
        std::cout << std::endl << game << std::endl;
    }
    return std::pair<int, int>(game.getHighestTile(), game.score);
}

/**
 * Creates and completes n-many games using the Expectimax simulation function.
 * Tabulates data from game each in order to display results at completion.
 */
int expectimaxSolve(int n, int depth, int display_level, 
    std::vector<int> *scores, std::vector<int> *highest_tiles) {

    int successes = 0;
    for (int i = 0; i < n; ++i) {
        std::pair<int, int> result = expectimaxSearch(depth, display_level, Game());
        if (result.first >= WIN) {
            successes++;
        }
        (*highest_tiles).push_back(result.first);
        (*scores).push_back(result.second);
    }
    return successes;
}