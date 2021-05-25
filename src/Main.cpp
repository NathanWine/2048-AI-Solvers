#include <string>
#include <algorithm>
#include "Game.hpp"
#include "MonteCarloSolver.hpp"

class CmdParser {
    // Command parser from: https://stackoverflow.com/a/868894/9306928
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
    // Default argument values
    int num_games = 1;
    int num_runs = 25;

    CmdParser input(argc, argv);
    if (input.cmdOptionExists("-h")) {
        std::cout << "Got an -h flag followed by: " << input.getCmdOption("-h") << std::endl;
    }
    if (input.cmdOptionExists("-n")) {
        num_games = std::stoi(input.getCmdOption("-n"));
    }
    if (input.cmdOptionExists("-r")) {
        num_runs = std::stoi(input.getCmdOption("-r"));
    }

    std::cout << "Number of games: " << num_games << std::endl << "Number of runs: " << num_runs << std::endl;

    monteCarloSolve(num_games, num_runs, 2048);

    return 0;
}