#pragma once

#include <vector>
#include <array>
#include <iostream>

class board {
    private:
        int cols;
        int rows;
        int human;
        int agent;
        int depth;
        bool game;
        int counter;
        int current_player;

    public:
        std::vector<std::vector<int> > grid;

        board();
        void copy(std::vector<std::vector<int> >);

        void init_board();
        void print_board();

        int spot_value(int, int, int);
        int container_value(std::vector<int>, int);
        int board_value(int);

        std::array<int, 2> minimax(int, int, int, int);
        void setDepth(int x) {depth = x;};

        void place_token(int, int);
        int player_move();

        bool game_over(int);
        void play();
};
