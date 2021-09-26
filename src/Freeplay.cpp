#include "Freeplay.hpp"
#include "Game.hpp"
#include "Util.hpp"
#include "Minimax.hpp"
#include <iostream>

bool Freeplay::move(char keycode, Game &game) {
    if (keycode == 'w' || keycode == 'W') {
        game.up(false);
    }
    else if (keycode == 'a' || keycode == 'A') {
        game.left(false);
    }
    else if (keycode == 'd' || keycode == 'D') {
        game.right(false);
    }
    else if (keycode == 's' || keycode == 'S') {
        game.down(false);
    }
    else {
        return false;
    }
    return true;
}

int Freeplay::play(int &num_games, std::vector<int> &scores, std::vector<int> &highest_tiles) {
    num_games = 1;
    int successes = 0;
    int width = 0;
    char input = 0;
    const std::string scoreLabel = "Score: ";

    Game game;
    std::string gamePrint = Game::staticPrint(game, &width);
    std::cout << "Score: " << game.score << "\n" << gamePrint;
    
    int bestMove = Minimax::calculateBestMove(1, 0, game);
    std::string moveLabel = "\nBest move: ";
    switch (bestMove) {
    case UP:
        moveLabel += "Up";
        break;
    case DOWN:
        moveLabel += "Down";
        break;
    case LEFT:
        moveLabel += "Left";
        break;
    case RIGHT:
        moveLabel += "Right";
        break;
    }
    std::cout << moveLabel;
    std::cout.flush();

    while(game.canContinue() && (input = Util::readCharacter(input))) {
        for (int i = 0; i < (int)moveLabel.length(); i++) {
            std::cout << "\b \b";
        }
        std::cout << "\033[A";
        for (int i = 0; i < DIM - 1; i++) {
            for (int j = 0; j < width; j++) {
                std::cout << "\b \b";
            }
            std::cout << "\033[A";
        }
        std::cout << "\033[A";
        for (int k = 0; k < ((int)scoreLabel.length() + Util::numDigits(game.score)); k++) {
            std::cout << "\b \b";
        }

        move(input, game);
        gamePrint = Game::staticPrint(game, &width);
        std::cout << "Score: " << game.score << "\n" << gamePrint;
        int res = Minimax::calculateBestMove(1, 0, game);
        moveLabel = "\nBest move: ";
        switch (res)
        {
        case UP:
            moveLabel += "Up";
            break;
        case DOWN:
            moveLabel += "Down";
            break;
        case LEFT:
            moveLabel += "Left";
            break;
        case RIGHT:
            moveLabel += "Right";
            break;
        }
        std::cout << moveLabel;
        std::cout.flush();
    }
    if (game.getHighestTile() >= 2048) {
        successes += 1;
    }
    scores.push_back(game.score);
    highest_tiles.push_back(game.getHighestTile());

    // Ask if the user wants to play again, increment num_games

    return successes;
}