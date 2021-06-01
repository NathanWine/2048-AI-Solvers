#include "Game.hpp"
#include "MonteCarloSolver.hpp"
#include "Minimax.hpp"

class CmdParser {
    // Simple cmd parser from: https://stackoverflow.com/a/868894/9306928
    public:
        CmdParser (int &argc, char **argv) {
            for (int i = 1; i < argc; ++i) {
                cmds.push_back(std::string(argv[i]));
            }
        }

        const std::string& getCmdOption(const std::string &opt) const {
            std::vector<std::string>::const_iterator itr;
            itr =  std::find(cmds.begin(), cmds.end(), opt);
            if (itr != cmds.end() && ++itr != cmds.end()){
                return *itr;
            }
            static const std::string empty_string("");
            return empty_string;
        }

        bool cmdOptionExists(const std::string &opt) const {
            return std::find(cmds.begin(), cmds.end(), opt) != cmds.end();
        }
    private:
        std::vector <std::string> cmds;
};

int main(int argc, char** argv) {
    // int num_games = 1;
    // int num_runs = 25;
    // int display_level = 1;

    /*CmdParser cmd_parser(argc, argv);
    if (cmd_parser.cmdOptionExists("-h") || cmd_parser.cmdOptionExists("--help") 
            || cmd_parser.cmdOptionExists("help") || cmd_parser.cmdOptionExists("usage")) {
        std::string msg = "Usage:\
            \n  MonteCarloSolver <flag> <flag_val> ...\
            \n  Flag list:\
            \n    -n: Integer value; # times to run the algorithm. Stats displayed at program completion\
            \n    -r: Integer value; # runs MonteCarlo completes for each move. Higher=better but slower. Recommend 10-100\
            \n    -d: Integer value; Display level. Higher=more display. 0=minimal, 1=medium, 2=full\
            \n  Ex: MonteCarloSolver -n 1 -r 50 -d 1";
        std::cout << msg << std::endl;
        return 0;
    }
    if (cmd_parser.cmdOptionExists("-n")) {
        num_games = std::stoi(cmd_parser.getCmdOption("-n"));
    }
    if (cmd_parser.cmdOptionExists("-r")) {
        num_runs = std::stoi(cmd_parser.getCmdOption("-r"));
    }
    if (cmd_parser.cmdOptionExists("-d")) {
        display_level = std::stoi(cmd_parser.getCmdOption("-d"));
    }*/

    // monteCarloSolve(num_games, num_runs, display_level, 2048);

    // Game game = Game();
    // game.state = {
    //     {2, 2, 0, 0},
    //     {0, 0, 0, 0},
    //     {0, 0, 0, 0},
    //     {0, 0, 0, 0},
    // };
    // std::cout << game << std::endl;
    // std::cout << "about to compute" << std::endl;
    // weightedmoves poss = game.computePossibilities();
    // std::cout << "done computing" << std::endl;
    // for (int i = 0; i < (int) poss.size(); ++i) {
    //     std::cout << "loop1" << std::endl;
    //     for (int j = 0; j < (int) poss[i].size(); ++j) {
    //         std::cout << "j: " << j << " | Move: " << i;
    //         std::cout << " | Prob: " << poss[i][j].first << std::endl;
            
    //     }
    // }

    miniMaxSolve(1, 1, 2, 2048);

    // Game game = Game();
    // game.state = {
    //     {4, 2, 8, 0},
    //     {2, 16, 0, 0},
    //     {8, 64, 0, 0},
    //     {2, 32, 2, 0},
    // };
    // weightedmoves possibilities = game.computePossibilities();

    // std::map<int, float> scores;
    // std::cout << "Possiblilities size: " << possibilities.size() << std::endl;
    // for (auto const& possibility : possibilities) {
    //     int move = possibility.first;
    //     std::vector<std::pair<float, Game>> weighted_subset = possibility.second;
    //     int len = (int) weighted_subset.size();
    //     if (len > 0) {
    //         scores[move] = 0.0;
    //         for (int j = 0; j < len; ++j) {
    //             scores[move] += minimaxScore(1, weighted_subset[j].second) * weighted_subset[j].first;
    //         }
    //     }
    // }

    // std::cout << "Size of scores: " << scores.size() << std::endl;

    // for (auto const& entry : scores) {
    //     std::cout << "first: " << entry.first << " | second: " << entry.second << std::endl;
    // }

    return 0;
}