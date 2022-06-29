#include "board.h"

int menu();

int main(int argc, char** argv) {
    board game;
    game.init_board();
    
    game.place_token(2,1);
    game.place_token(2,2);
    game.place_token(2,1);
    game.place_token(2,2);
    game.place_token(3,1);
    game.place_token(3,1);
    game.place_token(3,2);
    game.place_token(4,1);
    game.place_token(4,2);
    game.place_token(5,2);
    game.place_token(1,1);

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


