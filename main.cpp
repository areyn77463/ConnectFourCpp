#include "board.h"

int main(int argc, char** argv) {
    board game;
    game.init_board();
    for (int i = 0; i < 6; i++) {
    game.place_token(4, 1);
    }
    game.place_token(game.player_move(), 2);
    game.print_board();
    return 0;
}
