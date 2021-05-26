#include "Game.hpp"
#include "MonteCarloSolver.hpp"
#include "Heuristics.hpp"

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
    int num_games = 1;
    int num_runs = 25;
    int display_level = 1;

    CmdParser cms_parser(argc, argv);
    if (cms_parser.cmdOptionExists("-h") || cms_parser.cmdOptionExists("--help") 
            || cms_parser.cmdOptionExists("help") || cms_parser.cmdOptionExists("usage")) {
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
    if (cms_parser.cmdOptionExists("-n")) {
        num_games = std::stoi(cms_parser.getCmdOption("-n"));
    }
    if (cms_parser.cmdOptionExists("-r")) {
        num_runs = std::stoi(cms_parser.getCmdOption("-r"));
    }
    if (cms_parser.cmdOptionExists("-d")) {
        display_level = std::stoi(cms_parser.getCmdOption("-d"));
    }

    monteCarloSolve(num_games, num_runs, display_level, 2048);

    return 0;
}