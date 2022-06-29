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


/** @brief copy the contents of given board.grid to current board.grid
 *
 *  @param rhs is the given board to copy from
 */
void board::copy(std::vector<std::vector<int> > rhs) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <cols; j++) {
            grid[i][j] = rhs[i][j];
        }
    }
}


/** @brief initializes an empty vector of vectors to a 6x7 board of 0's. A '0' 
 *         is an empty/unplayed space
 *
 */
void board::init_board() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = 0;
        }
    }
}


/** @brief prints empty board with surrounding graphics. A number line is used 
 *         to show column positions and the board is outlined in '|'s and '-'s. 
 *         The unplayed spaces appear as ' ' (blank spaces)
 *
 */
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
    std::cout << std::setfill('-') << std::setw(15) << '-' << std::setfill(' ') 
              << "\n";
    std::cout << " 0 1 2 3 4 5 6 \n\n";
}


/** @brief given the position and player, the function will search up from the 
 *         bottom of the board at the selected column and insert a player or 
 *         agent token at the first open space
 *
 *  @param position is the column chosen to drop token
 *  @param player is whoevers turn it is (player or agent)
 *
 */
void board::place_token(int position, int player) {
    for (int i = rows - 1; i > -1; i--) {
        if (grid[i][position] == 0) {
            grid[i][position] = player;
            break;
        }
    }
}


/** @brief prompts player to choose a position to place their token. Also checks
 *         that the selected move is in the scope of the game. No letters, 
 *         symbols, 6 > position < 0, and no selecting columns that are full 
 *         without an error message. Will allow the player to move until a valid
 *         position is chosen.
 *
 */
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


/** @brief  
 *
 *  @param positive
 *  @param negative
 *  @param neutral
 *
 */
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


/** @brief
 *
 *  @param container
 *  @param player
 *
 */
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


/** @brief
 *
 *  @param player
 *
 */
bool board::game_over(int player) {
    //Check horizontal for four in row starting from bottom left
    //and moving right and up: direction = -
    for (int i = rows - 1; i > -1; i--) {
        int head = 0;
        int tail = 0;
        int count_four = 0;
        while (head < cols - 3) { 
            if((int)grid[i][tail] == player)
                count_four++;
            if(count_four == 4)
                return true;
            if (tail == (head + 3)) {
                head++;
                tail = head;
                count_four = 0;
            } else
                tail++;
        }
    }
    //Check diagonal bottom left to top right starting from bottom left
    //and moving to top right: direction = / 
    for (int i = rows - 1; i > rows - 3; i--) {
        int head = i;
        int tail = 0;
        int count_four = 0;
        while (tail < cols) {
            if((int)grid[head][tail] == player)
                count_four++;
            if(count_four == 4)
                return true;
            if(head == (i - 3)) {
                head = i;
                tail = tail - 2;
                count_four = 0; 
            }else {
                head--;
                tail++;
            }
        }
    }
    //Check vertical starting from bottom left and moving to top right
    //Direction = |
    for (int i = rows - 1; i > rows - 3; i--) {
        int head = i;
        int tail = 0;
        int count_four = 0;
        while (tail < cols) {
            if((int)grid[head][tail] == player)
                count_four++;
            if(count_four == 4)
                return true;
            if (head == (i - 3)) {
                head = i;
                tail++;
                count_four = 0;
            } else
                head--;
        }
    }
    //Check diagonal bottom right to top left starting from middle left
    //and moving to middle right: Direction '\' 
    for (int i = rows - 1; i > rows - 3; i--) {
        int head = i;
        int tail = cols - 3;
        int count_four = 0;
        while (tail < cols) {
            if((int)grid[head][tail] == player)
                count_four++;
            if(count_four == 4)
                return true;
            if(head == (i - 3)) {
                head = i;
                tail = tail + 4;
                count_four = 0; 
            }else {
                head--;
                tail--;
            }
        }
    }    
    return false;
}
            


