#include <cctype>
#include <chrono>
#include <numeric>
#include "Game.hpp"
#include "MonteCarlo.hpp"
#include "Minimax.hpp"
#include "Expectimax.hpp"

using namespace std::chrono;

// Simple cmd line parsing function to determine if a string is representing a number
bool isNumber(const std::string& str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) {
            return false;
        }
    }
    return true;
}

// Simple function to convert a string to lowercase
void lowercase(std::string *str) {
    std::transform((*str).begin(), (*str).end(), (*str).begin(), 
        [](unsigned char c){ return std::tolower(c); });
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
        std::vector<std::string> cmds;
};

int main(int argc, char** argv) {
    enum ALGORITHMS {MONTECARLO, MINIMAX, EXPECTIMAX};
    std::map<std::string, int> alg_map = {{"montecarlo", MONTECARLO}, {"minimax", MINIMAX}, {"expectimax", EXPECTIMAX}};
    int alg_key = MINIMAX;
    int num_games = 1;
    int num_runs = 25;
    int depth = 1;
    int print_level = 2;

    // Parse command-line arguments
    CmdParser cmd_parser(argc, argv);
    if (cmd_parser.cmdOptionExists("-h") || cmd_parser.cmdOptionExists("--help") 
            || cmd_parser.cmdOptionExists("help") || cmd_parser.cmdOptionExists("usage")) {
        std::string msg = "Usage:\
            \n  AISolver <flag> <flag_val> ...\
            \n  Flag list:\
            \n    -a: Integer/String value; Algorithm to run. 0=montecarlo, 1=minimax, 2=expectimax\
            \n    -n: Integer value; # times to run the algorithm. Stats displayed at program completion\
            \n    -r: Integer value; # runs MonteCarlo completes for each move. Higher=better but slower. Recommend 10-100\
            \n    -d: Integer value; # depth level for Minimax / Expectimax\
            \n    -p: Integer value; Print level. Higher=more display. 0=minimal, 1=medium, 2=high, 3=full\
            \n  Ex: AISolver -a minimax -n 1 -d 1 -p 3";
        std::cout << msg << std::endl;
        return 0;
    }
    if (cmd_parser.cmdOptionExists("-a")) {
        std::string a_arg = cmd_parser.getCmdOption("-a");
        if (isNumber(a_arg)) {
            alg_key = std::stoi(a_arg);
        }
        else {
            lowercase(&a_arg);
            alg_key = alg_map[a_arg];
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

    // Set up variables for statistics
    int successes = 0;
    std::vector<int> scores;
    std::vector<int> highest_tiles;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    switch (alg_key) {          // Run chosen algorithm
        case MONTECARLO:
            successes = monteCarloSolve(num_games, num_runs, print_level, &scores, &highest_tiles);
            break;
        case MINIMAX:
            successes = minimaxSolve(num_games, depth, print_level, &scores, &highest_tiles);
            break;
        case EXPECTIMAX:
            successes = expectimaxSolve(num_games, depth, print_level, &scores, &highest_tiles);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

    // Display statistics
    if (num_games > 1) {
        std::cout << "Success rate: " << (float) successes / num_games * 100 << "%" << std::endl;
        float average = std::accumulate(scores.begin(), scores.end(), 0.0) / num_games;
        std::cout << "Average score: " << average << std::endl;
        std::cout << "Highest tiles: ";
        for (int i = 0; i < (int) highest_tiles.size(); ++i) {
            std::cout << highest_tiles[i] << ", ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << (successes > 0 ? "Game won!" : "Game lost.") << std::endl;
        std::cout << "Final score: " << scores[0] << std::endl;
        std::cout << "Highest tile: " << highest_tiles[0] << std::endl;
    }
    std::cout << "Time elapsed: " << time_span.count() << " seconds" << std::endl;

    return 0;
}