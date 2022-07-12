#include "board.h"
#include <string>

const std::string ARGS = "-op -tp";

int main(int argc, char** argv) {
    std::size_t found;
    board game;
   
    if (argc == 1)
        game.play_one();
    else {
        std::string play = argv[1];
        found = ARGS.find(play);
        if (argc > 2 || play == "--help" || play == "-h") {
            std::cerr << "Use as: " << argv[0] << " [option]\n\n";
            std::cerr << "Options include : -op     -  One Player Mode (Default mode)\n"
                      << "                : -tp     -  Two Player Mode\n"
                      << "                : -h\n"
                      << "                  --help  -  Print this menu\n";
            std::cerr << "*No option will run as default mode*\n";
        }

        if (argc == 2 && found != std::string::npos) {
            if (play == "-op")
                game.play_one();
            if (play == "-tp")
                game.play_two();
        }
    }
    return 0;
}
