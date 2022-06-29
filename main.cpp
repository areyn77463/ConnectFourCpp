#include "board.h"

int menu();

int main(int argc, char** argv) {
    board game;
    game.init_board();
    for (int i = 0; i < 7; i++) {
        game.place_token(i, 1);
    }
    game.print_board();
    std::cout << game.game_over(1) << std::endl;
    return 0;
}

int menu() {
    int choice;
    std::cout << "Connect-4\n";
    std::cin >> choice;

    return choice;
}    


