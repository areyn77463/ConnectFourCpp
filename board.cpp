#include "board.h"

#include <iomanip>
#include <climits>

board::board() {
    cols = 7;
    rows = 6;
    human = 1;
    agent = 2;
    depth = 3;
    game = false;
    counter = 0;
    current_player = human;
    std::vector<std::vector<int> > board_construct(rows, std::vector<int>(cols));
    grid = board_construct;
}

void board::copy(std::vector<std::vector<int> > rhs) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <cols; j++) {
            grid[i][j] = rhs[i][j];
        }
    }
}

void board::init_board() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = 0;
        }
    }
}

void board::print_board() {
std::cout << " 0 1 2 3 4 5 6 \n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j == 0)
                std::cout << '|';
            switch (grid[i][j]) {
                case 0:
                    std::cout << ' ';
                    break;
                case 1:
                    std::cout << 'R';
                    break;
                case 2:
                    std::cout << 'Y';
                    break;
            }
            if (j != cols-1)
                std::cout << ' ';
            if (j+1 == cols)
                std::cout << '|';
        }
        std::cout << "\n";
    }
    std::cout << std::setfill('-') << std::setw(15) << '-' << std::setfill(' ') << "\n";
    std::cout << " 0 1 2 3 4 5 6 \n\n";
}

void board::place_token(int position, int player) {
    for (int i = rows - 1; i > -1; i--) {
        if (grid[i][position] == 0) {
        grid[i][position] = player;
        break;
        }
    }
}

int board::player_move() {
    int position = -1;
    while (true) {
        std::cout << "Player choose position: ";
        std::cin >> position;
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            std::cout << "That is not a column number!";
        } else if (!((int)position < cols && position >= 0)) {
            std::cout << "That is outside of the board!";
        } else if (grid[0][position] != 0) {
            std::cout << "That column is full!";
        } else
            break;
        std::cout << '\n' << '\n';
    }
    return position;
}

int board::spot_value(int positive, int negative, int neutral) {
    int spot_score = 0;
    if (positive == 4)
        spot_score += 10001;
    else if (positive == 3 && neutral == 1)
        spot_score += 1000;
    else if (positive == 2 && neutral == 2)
        spot_score += 100;
    else if (negative == 2 && neutral == 2)
        spot_score -= 101;
    else if (negative == 3 && neutral == 1)
        spot_score -= 1000;
    else if (negative == 4)
        spot_score -= 10000;
    return spot_score;
}

int board::container_value(std::vector<int> container, int player) {
    int reward = 0;
    int punish = 0;
    int empty_spot = 0;
    for (int i = 0; i < container.size(); i++) {
        reward += (container[i] == player) ? 1 : 0;
        punish += (container[i] == human || container[i] == agent) ? 1 : 0;
        empty_spot += (container[i] == 0) ? 1 : 0;
    }
    punish -= reward;
    return spot_value(reward, punish, empty_spot);
}
