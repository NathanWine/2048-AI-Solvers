#include <cctype>
#include "Game.hpp"
#include "MonteCarloSolver.hpp"
#include "Minimax.hpp"

bool isNumber(const std::string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

// Simple cmd parser from: https://stackoverflow.com/a/868894/9306928
class CmdParser {
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
    std::map<std::string, int> alg_map = {{"montecarlo", 0}, {"minimax", 1}};
    int algorithm = 10;
    int num_games = 1;
    int num_runs = 25;
    int depth = 1;
    int print_level = 2;

    CmdParser cmd_parser(argc, argv);
    if (cmd_parser.cmdOptionExists("-h") || cmd_parser.cmdOptionExists("--help") 
            || cmd_parser.cmdOptionExists("help") || cmd_parser.cmdOptionExists("usage")) {
        std::string msg = "Usage:\
            \n  MonteCarloSolver <flag> <flag_val> ...\
            \n  Flag list:\
            \n    -a: Integer/String value; Algorithm to run. 0=montecarlo, 1=minimax\
            \n    -n: Integer value; # times to run the algorithm. Stats displayed at program completion\
            \n    -r: Integer value; # runs MonteCarlo completes for each move. Higher=better but slower. Recommend 10-100\
            \n    -d: Integer value; # depth level for MiniMax / Expectimax\
            \n    -p: Integer value; Print level. Higher=more display. 0=minimal, 1=medium, 2=high, 3=full\
            \n  Ex: AISolver -a minimax -n 1 -r 50 -d 1";
        std::cout << msg << std::endl;
        return 0;
    }
    if (cmd_parser.cmdOptionExists("-a")) {
        std::string a_arg = cmd_parser.getCmdOption("-a");
        if (isNumber(a_arg)) {
            algorithm = std::stoi(a_arg);
        }
        else {
            algorithm = alg_map[a_arg];
        }
    }
    if (cmd_parser.cmdOptionExists("-n")) {
        std::string n_option = cmd_parser.getCmdOption("-n");
        if (isNumber(n_option)) {
            num_games = std::stoi(n_option);
        }
    }
    if (cmd_parser.cmdOptionExists("-r")) {
        std::string r_option = cmd_parser.getCmdOption("-r");
        if (isNumber(r_option)) {
            num_runs = std::stoi(r_option);
        }
    }
    if (cmd_parser.cmdOptionExists("-d")) {
        std::string d_option = cmd_parser.getCmdOption("-d");
        if (isNumber(d_option)) {
            depth = std::stoi(d_option);
        }
    }
    if (cmd_parser.cmdOptionExists("-p")) {
        std::string p_option = cmd_parser.getCmdOption("-p");
        if (isNumber(p_option)) {
            print_level = std::stoi(p_option);
        }
    }

    switch (algorithm) {
        case 0:
            monteCarloSolve(num_games, num_runs, print_level, 2048);
            break;
        case 1:
            miniMaxSolve(num_games, depth, print_level, 2048);
    }

    return 0;
}