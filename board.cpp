#include "board.h"

#include <algorithm>
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
        std::cout << "Quit any time with \"81\"\n";
        std::cout << "Player choose position: ";
        std::cin >> position;
        if (position == 81)
            exit(0);
        else if (!std::cin) {
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


/** @brief function scores a set of 4 spaces based on how many spaces
 *          are held by the current player, opposition, and  empty spots 
 *
 *  @param positive the spots filled by the player
 *  @param negative the spots filled by the opposition
 *  @param neutral the spots that are empty
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
        spot_score -= 1001;
    else if (negative == 4)
        spot_score -= 10000;
    return spot_score;
}


/** @brief function determines how many of each space is in the container
 *          - reward for spot being the player
 *          - punish for a spot being taken
 *          - empty_spot if an a spot is empty
 *
 *         punish -= reward corrects adding point to punish for spot held by current player 
 *          *could be avoided with longer condition statement at punish
 *
 *  @param container a set of 4 places on the board
 *  @param player the current player
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


/** @brief function stores sets of four pieces in all directions in container and sends them
 *         container_value function to be scored. the sum of all containers
 *         is the value of the board.
 *
 *  @param player the current player
 *
 *  @return board_score the value of the current board after scoring all of the tokens on it
 *
 */
int board::board_value(int player){
    int score = 0;
    std::vector<int> vCol(cols);
    std::vector<int> vRow(rows);
    std::vector<int> set(4);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {    //assign each value x axis on list
            vCol[j] = grid[i][j];
        }
        for (int j = 0; j < cols - 3; j++) {    
            for (int k = 0; k < 4; k++) {   //cut into sets of 4
                set[k] = vCol[j + k];
            }
            score += container_value(set, player);    //check the score of the set of 4 spaces
        }
    }
    //assign, cut, and check vertical sets of 4
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            vRow[j] = grid[j][i];
        }
        for (int j = 0; j < rows - 3; j++) {
            for (int k = 0; k < 4; k++) {
                set[k] = vRow[j + k];
            }
            score += container_value(set, player);
        }
    }
    //assign cut, and check diagonal sets of 4
    for (int i = 0; i < rows - 3; i++) {
        for (int j = 0; j < cols; j++) {
            vCol[j] = grid[i][j];
        }
        for (int j = 0; j < cols - 3; j++) {
            for (int k = 0; k < 4; k++) {
                set[k] = grid[i + k][j + k];
            }
            score += container_value(set, player);
        }
    }
    for (int i = 0; i < rows - 3; i++) {
        for (int j = 0; j < cols; j++) {
            vCol[j] = grid[i][j];
        }
        for (int j = 0; j < cols - 3; j++) {
            for (int k = 0; k < 4; k++) {
                set[k] = grid[i + 3 - k][j + k];
            }
            score += container_value(set, player);
        }
    }
    return score;
}


//attempted using game_over logic instead of nested for loops
/*int board::board_value(int player) {
    int board_score = 0;
    for (int i = rows - 1; i > -1; i--) {
        std::vector<int> container;
        int head = 0;
        int tail = 0;
        while (head < cols - 3) {
           container.push_back(grid[i][tail]);
           if (tail == (head + 3)) {
                board_score += container_value(container, player); 
                head++;
                tail = head;
                container.clear();
           } else  
                tail++;
        }
    }
    for (int i = rows - 1; i > rows - 4; i--) {
        std::vector<int> container;
        int head = i;
        int tail = 0;
        while (tail < cols) {
            container.push_back(grid[head][tail]);
            if(head == (i - 3)) {
                board_score += container_value(container, player);
                head = i;
                tail = tail - 2;
                container.clear(); 
            }else {
                head--;
                tail++;
            }
        }
    }
    for (int i = rows - 1; i > rows - 4; i--) {
        std::vector<int> container;
        int head = i;
        int tail = 0;
        while (tail < cols) {
            container.push_back(grid[head][tail]);
            if (head == (i - 3)) {
                board_score = container_value(container, player);
                head = i;
                tail++;
                container.clear();
            } else
                head--;
        }
    }
    for (int i = rows - 1; i > rows - 4; i--) {
        std::vector<int> container;
        int head = i;
        int tail = cols - 4;
        while (tail < cols) {
            container.push_back(grid[head][tail]);
            if(head == (i - 3)) {
                board_score = container_value(container, player);
                head = i;
                tail = tail + 4;
                container.clear(); 
            }else {
                head--;
                tail--;
            }
        }
    }     
   return board_score; 
}*/


/** @brief checks in all directions to find 4 of the same tokens in a row
 *
 *  @param player is whoevers turn it is (player or agent)
 *
 *  @return true if 4 of same tokens found in a row
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
    for (int i = rows - 1; i > rows - 4; i--) {
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
    for (int i = rows - 1; i > rows - 4; i--) {
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
    for (int i = rows - 1; i > rows - 4; i--) {
        int head = i;
        int tail = cols - 4;
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
            

/** @brief
 *
 * @param depth
 * @param player
 *
 */
std::array<int, 2> board::minimax(int depth,int alpha, int beta, int player) {
    if (depth == 0 || depth >= (rows*cols) - counter)
        return std::array<int, 2>{board_value(agent), -1}; 
    if (player == agent) {
        std::array<int, 2> score_move = {INT_MIN, -1};
        if (game_over(human))
            return score_move; 
        for (int i = 0; i < cols; i++) {
            if (grid[0][i] == 0) {
                board temp;
                temp.copy(grid);
                temp.place_token(i, player);
                int score = temp.minimax(depth - 1, alpha, beta, human)[0];
                if (score > score_move[0])
                    score_move = {score, i};
            alpha = std::max(alpha, score_move[0]);
            if (alpha >= beta)
                break;
            }
        }
        return score_move;
    } else {
        std::array<int, 2> score_move = {INT_MAX, -1};
        if (game_over(agent))
            return score_move; 
        for (int i = 0; i < cols; i++) {
            if (grid[0][i] == 0) {
                board temp;
                temp.copy(grid);
                temp.place_token(i, player);
                int score = temp.minimax(depth - 1, alpha, beta, agent)[0];
                if (score < score_move[0])
                    score_move = {score, i};
            beta = std::min(beta, score_move[0]);
            if (alpha >= beta)
                break;
            }
        }
        return score_move;
    }
}



/** @brief starts a game for 2 human players
 *
 */
void board::play_two() {
    init_board();
    print_board();
    while (!game) {
        if (current_player == agent) {
            std::cout << "\nPlayer 2\n";
            place_token(player_move(), agent);
        } else if (current_player == human) {
            std::cout << "\nPlayer 1\n";
            place_token(player_move(), human);
        } else if (counter == rows*cols)
            game = true;
        game = game_over(current_player);
        current_player = (current_player == 1) ? 2 : 1;
        counter++;
        std::cout << "\n";
        print_board();
    }
    if (counter == rows*cols)
        std::cout << "Draw\n";
    else
        std::cout << ((current_player == human) ? "Player 2 wins\n" : "Player 1 wins\n");
}


/** @brief starts a game for 2 human players
 *
 */
void board::play_one() {
    init_board();
    print_board();
    while (!game) {
        if (current_player == agent) {
            place_token(minimax(depth, INT_MIN, INT_MAX, agent)[1], agent);
        } else if (current_player == human) {
            std::cout << "\nPlayer 1\n";
            place_token(player_move(), human);
        } else if (counter == rows*cols)
            game = true;
        game = game_over(current_player);
        current_player = (current_player == 1) ? 2 : 1;
        counter++;
        std::cout << "\n";
        print_board();
    }
    if (counter == rows*cols)
        std::cout << "Draw\n\n";
    else
        std::cout << ((current_player == human) ? "Computer wins\n\n" : "Player 1 wins\n\n");
}
