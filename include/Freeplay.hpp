#ifndef FREEPLAY_H
#define FREEPLAY_H

class Game;

class Freeplay {
    private:
        bool move(char keycode, Game &game);
    protected:
    public:
        int play(int &num_games, std::vector<int> &scores, std::vector<int> &highest_tiles);
};

#endif