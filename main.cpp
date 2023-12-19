#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <chrono>
using namespace std;

//initialize global variables
bool close = false;
const string line = "#################################################################"; //draw a line in the command line
const string box = "< >"; // this will symbolize empty tiles
const int num_cols = 7; // not allowed to change number of columns
const int max_difficulty = 3; // maximum cpu difficulty
array<int, 2> scoreboard = {0, 0};

// these will be able to be modified by the settings() function
int num_rows = 6;
string player_1 = " o "; // player 1's token, can be changed to a different string in settings() (when playing vs cpu, the user is always player 1)
string player_2 = " x "; // player 2's token, can be changed to a different string in settings()
int cpu_difficulty = 1; // difficulty of computer opponent, 0 is easiest

// funny easter egg
// doing some things might cause the game to lose patience and turn on menal responses
bool patience = true;
// planned easter egg: entering an invalid move has a 50% chance to cause the game to lose patience


// the board
std::vector<std::vector<string>> board;

// for the coinflips
// random_device rd;
mt19937 gen(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
uniform_int_distribution<> distribution(1, 1000);
uniform_int_distribution<> distribution7(1, 7);

// coinflip that doesn't print anything
bool patienceflip() {
    return distribution(gen) % 2 == 0;
}

// choose a random column
int rand_col() {
    return distribution7(gen);
}

// create an empty board (clears out existing board if board size changed)
// initializes global board variable
void create_board() {
    board.resize(num_rows, std::vector<string>(num_cols, box));
}

// empty the board
void clear_board() {
    for (auto& row : board) {
        for (auto& element : row) {
        element = box;
    }
    }
}

// check if board is full
bool board_full() {
    for (const auto& row : board) {
        for (const auto& element : row) {
            if (element == box) {
                return false;
            }
        }
    }
    return true;
}

void display_column_labels() {
    cout << "----------------------------\n";
    for (const auto& item : {1, 2, 3, 4, 5, 6, 7}) {
        cout << '|' << item << "| ";
    }
    cout << "\n----------------------------\n";
    }
// show the current board state
void display_board(const vector<vector<string>>& brd) {
    display_column_labels();
    for (const auto& row : brd) {
        for (const auto& element : row) {
            cout << element << " ";
        }
        cout << endl;
    }
    display_column_labels();
}

void smile() {
    player_1 = ":^)";
    player_2 = "D:<";
}

void display_scoreboard() {
    cout << "Current score:\nPlayer 1 ---- " << scoreboard[0] << "\nPlayer 2 ---- " << scoreboard[1] << "\n";
    if (scoreboard[0] > scoreboard[1]) {
        cout << "Player 1 is in the lead!\n";
    } else if (scoreboard[0] == scoreboard[1]) {
        cout << "Looks like it's a draw!\n";
    } else {
        cout << "Player 2 is in the lead!\n";
    }
    cout << line << '\n';

}

void display_settings(string setting, auto value){
    cout << setting << " ---- " << value << '\n';
}

//rules of connect 4
// this is the intended behaviour of the game
void display_rules() {
    cout << "NOTE: The intended behaviour of this Connect4 game is shown below. If anything is not working as intended, please hmu.\n";
    cout << "\nObjectives of Connect4:\nTo win a game of Connect4, you must form a row, column or diagonal of 4 of your own pieces.\nIf your opponent manages to do this, or you facilitate them in doing so, you will lose the game.\n";
    cout << "\nGamemodes:\nSingleplayer ---- Play versus a CPU. Acess the settings menu to change CPU difficulty.\n";
    cout << "2-player ---- Play versus a human (using the same keyboard) or yourself.\n";
    cout << "0-player ---- Planned gamemode where you can pit two cpu players against each other (NOT IMPLEMENTED). \n";
    cout << "\nHow to play:\n- A coin will be flipped at the start of the match to determine which player goes first.\n- When playing against CPU, you will be designated as Player 1.\n";
    cout << "- Each 'ROUND', one player must make a VALID move.\n- If it is your turn to play, you can either choose to drop a piece into the board from the top, or pop off (remove) one of your pieces from the bottom.\n";
    cout << "- When removing pieces, remember that gravity exists.\n- Note that you may freely change your mind during your turn, but once you select a column to affect, the decision cannot be undone.\n";
    cout << "\nRules:\n1. You are only allowed to 'POP' your own pieces.\n";
    cout << "2. You are only allowed to drop pieces in columns if there is an empty space at the top.\n";
    cout << "3. Each match of Connect4 can only have 1 winner. Note that draws are made impossible due to the ability to remove pieces.\n";
    cout << "4. You will lose if your move forms a 4-in-a-row for your opponent, even if you form a 4-in-a-row of your own token.\n";
}

void display_controls() {
    cout << "Game controls:\n";
    cout << "/pop ---- Switch to pop mode to remove a piece on the bottom row.\n";
    cout << "/drop ---- Switch to drop mode to drop a piece along the top row.\n";
    cout << "1, 2, 3, 5, 6 or 7 ---- Select a column to either pop or drop, depending on which mode is selected.\n";
    cout << "/quit ---- Forfeit the match.\n";
    cout << "\nOther things of note:\n- 'DROP' mode is the default at the start of each round.\n- You will be required to confrim your decision to /quit.\n";
}

// check if a string contains only spaces
bool contains_only_space(string str) {
    for (char ch : str) {
        if (ch != ' ') {
            return false;  // No need to continue checking once a non-space is found
        }
    }
    return true;
}

// handles the settings menu and its associated commands
void settings() {
    string user_input;
    do {
        cout << "\nCurrent settings:\n";
        display_settings("(1) Number of rows", num_rows);
        display_settings("(2) Player 1 symbol", player_1);
        display_settings("(3) Player 2 symbol", player_2);
        display_settings("(4) CPU difficulty level", cpu_difficulty);
        cout << "\nPlease input (copy) the name of the setting you want to change or its number. \nInput '/back' to return to the main menu. \nInput '/default' to revert to default settings.\n";
        cout << line << '\n';
        getline(std::cin, user_input);

        if (user_input == "/back") {
            continue;
        } else if (user_input == "Number of rows" || user_input == "1") {
            string input_rows;
            cout << "How many rows would you like? (You can only have 4 to 10 rows)\n";
            getline(cin, input_rows);
            try {
                num_rows = stoi(input_rows);
            } catch (const std::exception& e) {
                cout << "Please enter an integer only!\n";
                cout << line << '\n';
                continue;
            }

            // limit range of integers
            if (num_rows > 10) {
                num_rows = 10;
                cout << "Maximum number of 10 rows set.\n";
                cout << line << '\n';
            } else if (num_rows < 4) {
                num_rows = 4;
                cout << "Minimum number of 4 rows set.\n";
                cout << line << '\n';
            } else {
                cout << "Number of rows set to " << num_rows << ".\n";
                cout << line << '\n';
            }
        } else if (user_input == "Player 1 symbol" || user_input == "2") {
            const int max_symlength = box.length()+2;
            string new_sym;
            cout << "Please input your desired symbol for Player 1.\nFor best effect, symbol length should be 3 characters (including spaces).\n";
            cout << "Keep in mind that your symbol choice is purely for your own visual purposes. \n";
            cout << "The CPU player will not get confused, but you might. \n" << line << '\n';
            //cin.ignore();
            getline(cin, new_sym);
            if (new_sym == player_2) {
                cout << "Player 2 has already chosen " << new_sym << " as their symbol. Try having some originality.\n";
                cout << line << '\n';
                continue;
            } else if (new_sym.length() > max_symlength) {
                cout << "Symbol cannot be longer than " << (max_symlength) << " characters!\n";
                cout << line << '\n';
                continue;
            } else if (new_sym.empty()) {
                cout << "Symbol cannot be empty!\n";
                cout << line << '\n';
                continue;
            } else if (contains_only_space(new_sym)) {
                if (patience){
                    cout << "Symbol cannot be empty!\n";
                    cout << line << '\n';
                    if (patienceflip()){
                    patience = false;
                    }
                    continue;
                } else {
                    cout << "Dude seriously not cool, your symbol can't be empty. Stop trying to get around the system.\n";
                    cout << line << '\n';
                    continue;
                }
            } else if (new_sym == box) {
                cout << "Wow we got a rael comedian over here. Nice try scrub.\n";
                cout << line << '\n';
                continue;
            } else {
                player_1 = new_sym;
                cout << "Player 1 symbol set to " << player_1 << ".\n";
                cout << line << '\n';
                patience = true;
            }
        } else if (user_input == "Player 2 symbol" || user_input == "3") {
            const int max_symlength = box.length()+2;
            string new_sym;
            cout << "Please input your desired symbol for Player 2.\nFor best effect, symbol length should be 3 characters (including spaces).\n";
            cout << "Keep in mind that your symbol choice is purely for your own visual purposes. \n";
            cout << "The CPU player will not get confused, but you might. \n" << line << '\n';
            //cin.ignore();
            getline(cin, new_sym);
            if (new_sym == player_1) {
                cout << "Player 1 has already chosen " << new_sym << " as their symbol. Try having some originality.\n";
                cout << line << '\n';
                continue;
            } else if (new_sym.length() > max_symlength) {
                cout << "Symbol cannot be longer than " << (max_symlength) << " characters!\n";
                cout << line << '\n';
                continue;
            } else if (new_sym.empty()) {
                cout << "Symbol cannot be empty!\n";
                cout << line << '\n';
                continue;
            } else if (contains_only_space(new_sym)) {
                if (patience){
                    cout << "Symbol cannot be empty!\n";
                    cout << line << '\n';
                    if (patienceflip()){
                    patience = false;
                    }
                    continue;
                } else {
                    cout << "Dude seriously not cool, your symbol can't be empty. Stop trying to get around the system.\n";
                    cout << line << '\n';
                    continue;
                }
            } else if (new_sym == box) {
                cout << "Wow we got a rael comedian over here. Nice try scrub.\n";
                cout << line << '\n';
                continue;
            } else {
                player_2 = new_sym;
                cout << "Player 2 symbol set to " << player_2 << ".\n";
                cout << line << '\n';
                patience = true;
            }
            
        } else if (user_input == "CPU difficulty level" || user_input == "4") {
            string new_diff;
            cout << line << "\nDifficulty can only range from 1 to "<< max_difficulty <<" , with 1 being the easiest and " << max_difficulty << " being the hardest.\n";
            cout << "\nCurrently implemented difficulty levels:\n(1) ---- Makes random moves, only becomes a tryhard at the last moment. Cannot use POP moves unless board is full.\n";
            cout << "(2) ---- Uses a basic strategy to increase its chances of winning. Allowed to use pop moves (I just need to figure out how).\n";
            cout << "\nPlease input the difficulty level of the CPU player.\n" << line << '\n';
            getline(cin, new_diff);
            try {
                cpu_difficulty = stoi(new_diff);
            } catch (const std::exception& e) {
                cout << "Please enter an integer only!\n";
                cout << line << '\n';
                continue;
            }
            if (cpu_difficulty > max_difficulty) {
                cpu_difficulty = max_difficulty;
                cout << "Maximum difficulty level of " << max_difficulty << " set.\n";
                cout << line << '\n';
            } else if (cpu_difficulty < 1) {
                cpu_difficulty = 1;
                cout << "Minimum difficulty level of 1 set.\n";
                cout << line << '\n';
            } else {
                cout << "CPU difficulty level set to " << cpu_difficulty << ".\n";
                cout << line << '\n';
            }

        } else if (user_input == "/default") {
            num_rows = 6;
            player_1 = "1";
            player_2 = "2";
            cpu_difficulty = 1;
            cout << "Settings have been reset to defaults.\n";
            cout << line << '\n';
        } else {
            cout << "'" << user_input << "'" << "is neither a valid command nor is it an available option.\n";
            cout << line << '\n';
        }
            
            
        } while (user_input != "/back");

}



// applies gravity to the entire board
void gravity() {
    //apply gravity the same number of times as the number of rows
    int nrows = num_rows;
    while (nrows > 0) {    
        bool skip_row = true;
        vector<string> prev_row;
        int row_index = 0;
        for (auto& row : board) {
            if (skip_row) {
                skip_row = false; // skip first row
            } else {
                int col_index = 0;
                for (auto& element : row) {
                if (element == box && prev_row[col_index] != box) {
                    element = prev_row[col_index];
                    board[row_index-1][col_index] = box;
                }
                col_index++;
            }
            }
            prev_row = row;
            row_index++;
        }
        nrows -= 1;
    }
}

// drop a piece at column_number
//return true only if a sucessful move has been made
bool drop_piece(int column_number, string player_sym) {
    // change board at top row (drop a piece there) if there is space
    if (board[0][column_number-1] == box) {
        board[0][column_number-1] = player_sym;
        //apply gravity
        gravity();
        return true;
    } else {
        cout << " Your piece met some resistance! Try another move.\n";
        return false;
    }
    
    
}

//returns true only if a successful move has been made
//players can only pop their own pieces
bool pop_piece(int column_number, string player_sym)  {
    // check if move is legal
    if (board[num_rows-1][column_number-1] == player_sym) {
        board[num_rows-1][column_number-1] = box;
        //apply gravity
        gravity();
    } else {
        cout << "You can only pop your own pieces! Try another move.\n";
        return false;
    }
}

/*
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> distribution(1, 100);
*/
// returns true if player 1 wins the coinflip
bool coinflip() {
    bool player1Wins = distribution(gen) % 2 == 0;

    if (player1Wins) {
        cout << "Player 1 won the coinflip!\n";
    } else {
        cout << "Player 2 won the coinflip!\n";
    }

    return player1Wins;
}

void turn_display(int player, int round) {
    cout << "###############   ROUND " << round << "   ###############\n";
    cout << "It is currently player " << player <<"'s turn!\n";
    cout << "-------------------------------------------\n";

}

struct status {
    bool checkwinner;
    int winner_id;
}; // need this semicolon

status check_upright_diag() {
    status result;
    const int start_row_index = 3;
    const int max_col_idx = num_cols-3;
    bool player_1_victory = false;
    bool player_2_victory = false;

    int row_index = 0;
    for (const auto& row : board) {
        if (row_index < start_row_index) {
            row_index++;
            continue;
        }
        int col_index = 0;
        for (const auto& element : row) {
            int in_a_urdiag = 1;
            if (col_index == max_col_idx) {
                break;
            } else if (element != box) {
                while (board[row_index - in_a_urdiag][col_index + in_a_urdiag] == element) {
                    in_a_urdiag++;
                }
                if (in_a_urdiag > 3 && element == player_1) {
                    player_1_victory = true;
                } else if (in_a_urdiag > 3 && element == player_2) {
                    player_2_victory = true;
                }
            }
            col_index++;
        }
        row_index++;
    }
    if (player_1_victory && player_2_victory) {
        result.checkwinner = true;
        result.winner_id = 3;
        return result;
    } else if (player_1_victory) {
        result.checkwinner = true;
        result.winner_id = 1;
        return result;
    } else if (player_2_victory) {
        result.checkwinner = true;
        result.winner_id = 2;
        return result;
    } else {
        result.checkwinner = false;
        result.winner_id = 0;
        return result;
    }
}

status check_horizontal() {
    status result;
    const int max_col_idx = num_cols-3;
    bool player_1_victory = false;
    bool player_2_victory = false;

    int row_index = 0;
    for (const auto& row : board) {
        int col_index = 0;
        for (const auto& element : row) {
            int in_a_row = 1;
            if (col_index == max_col_idx) {
                break;
            } else if (element != box) {
                while (board[row_index][col_index + in_a_row] == element) {
                    in_a_row++;
                }
                if (in_a_row > 3 && element == player_1) {
                    player_1_victory = true;
                } else if (in_a_row > 3 && element == player_2) {
                    player_2_victory = true;
                }
            }
            col_index++;
        }
        row_index++;
    }
    if (player_1_victory && player_2_victory) {
        result.checkwinner = true;
        result.winner_id = 3;
        return result;
    } else if (player_1_victory) {
        result.checkwinner = true;
        result.winner_id = 1;
        return result;
    } else if (player_2_victory) {
        result.checkwinner = true;
        result.winner_id = 2;
        return result;
    } else {
        result.checkwinner = false;
        result.winner_id = 0;
        return result;
    }
}

status check_vertical() {
    status result;
    const int start_row_index = 3;
    bool player_1_victory = false;
    bool player_2_victory = false;

    int row_index = 0;
    for (const auto& row : board) {
        if (row_index < start_row_index) {
            row_index++;
            continue;
        }
        int col_index = 0;
        for (const auto& element : row) {
            int in_a_col = 1;
            if (element != box) {
                while (board[row_index - in_a_col][col_index] == element) {
                    in_a_col++;
                }
                if (in_a_col > 3 && element == player_1) {
                    player_1_victory = true;
                } else if (in_a_col > 3 && element == player_2) {
                    player_2_victory = true;
                }
            }
            col_index++;
        }
        row_index++;
    }
    if (player_1_victory && player_2_victory) {
        result.checkwinner = true;
        result.winner_id = 3;
        return result;
    } else if (player_1_victory) {
        result.checkwinner = true;
        result.winner_id = 1;
        return result;
    } else if (player_2_victory) {
        result.checkwinner = true;
        result.winner_id = 2;
        return result;
    } else {
        result.checkwinner = false;
        result.winner_id = 0;
        return result;
    }
}

status check_upleft_diag() {
    status result;
    const int start_row_index = 3;
    const int start_col_index = 3;
    bool player_1_victory = false;
    bool player_2_victory = false;

    int row_index = 0;
    for (const auto& row : board) {
        if (row_index < start_row_index) {
            row_index++;
            continue;
        }
        int col_index = 0;
        //cout << "checking row index " << row_index << '\n';
        for (const auto& element : row) {
            int in_a_uldiag = 1;
            if (col_index < start_col_index) {
                col_index++;
                continue;
            } else if (element != box) {
                //cout << "checking row index " << row_index << " col index " << col_index << '\n';
                while (board[row_index - in_a_uldiag][col_index - in_a_uldiag] == element) {
                    in_a_uldiag++;
                }
                if (in_a_uldiag > 3 && element == player_1) {
                    player_1_victory = true;
                } else if (in_a_uldiag > 3 && element == player_2) {
                    player_2_victory = true;
                }
            }
            col_index++;
        }
        row_index++;
    }
    if (player_1_victory && player_2_victory) {
        result.checkwinner = true;
        result.winner_id = 3;
        return result;
    } else if (player_1_victory) {
        result.checkwinner = true;
        result.winner_id = 1;
        return result;
    } else if (player_2_victory) {
        result.checkwinner = true;
        result.winner_id = 2;
        return result;
    } else {
        result.checkwinner = false;
        result.winner_id = 0;
        return result;
    }
}

//winner_id = 0 means nobody wins yet
//winner_id = 1 means player 1 wins
//winner_id = 2 means player 2 wins
//winner_id = 3 means both players win
status checkwin() {
    status final_result;
    const int start_row_idx = 3;
    const int max_col_idx = num_cols-4;

    bool player_1_victory = false;
    bool player_2_victory = false;
    // some code that checks if anyone has won

    //first check up right diagonals
    status ur_result = check_upright_diag();
    if (ur_result.winner_id == 3) {
        return ur_result; // return if two parallel rows
    }

    status ul_result = check_upleft_diag();
    if (ur_result.winner_id == 3) {
        return ur_result; // return if two parallel rows
    }

    status horizontal_result = check_horizontal();
    if (horizontal_result.winner_id == 3) {
        return horizontal_result; // return if two parallel rows
    }

    status vertical_result = check_vertical();
    if (vertical_result.winner_id == 3) {
        return vertical_result; // return if two parallel rows
    }

    array<status, 4> result_array = {ur_result, ul_result, horizontal_result, vertical_result};
    // int id_sum = 0;
    int current_winner_id = 0;
    // if all tests agree that nobody has won
    for (status result : result_array) {
        // id_sum += result.winner_id;
        if (result.checkwinner && current_winner_id == 0) {
            current_winner_id = result.winner_id;
        } else if (result.checkwinner && current_winner_id != result.winner_id) {
            current_winner_id = 3;
        }
    }
    final_result.winner_id = current_winner_id;
    final_result.checkwinner = (current_winner_id > 0);
    return final_result;

    // use result.checkwinner = true if somehas won
    // use result.winner_id to store who won 
}
// handles the gameloop for 2 player mode

void two_player_gameloop() {
    create_board();
    // roll a dice to decide who goes first
    bool player_1_goes_first = coinflip();
    cout << "Use /quit as your turn action to forfeit the current match and return to gamemode selection.\n";
    int player_id; // who is the current player?
    if (player_1_goes_first) {
        player_id = 1;
    } else {
        player_id = 2;
    }
    bool check_win = false;
    int round_num = 1; 
    string action;
    bool pop = false;
    do {
        string action;
        string symb = player_2;
        int next_player_id = 1; // opponent's id
        bool valid_move;
        if (player_id == 1){
            symb = player_1;
            next_player_id = 2;
        }

        string mode = "drop";
        if (pop) {
            mode = "pop";
        }

        turn_display(player_id, round_num);
        display_board(board);
        cout << "Player " << player_id << " please make your move.\nYour symbol is '" << symb <<"'.\n";
        cout << "You are currently in '" << mode << "' mode.\n" << line << '\n';
        getline(cin, action);
        if (action == "/quit") {
            string quitt;
            cout << "Are you sure you want to forfeit the current match?\nYour opponent will be awarded a win if you forfeit.\nEnter 'yes' if you really want to quit.\n";
            cout << line <<'\n';
            getline(cin, quitt);
            if (quitt == "yes") {
                cout << "Player " << player_id << " forefeits!\n" << line << '\n';
                scoreboard[next_player_id-1]++;
                display_scoreboard();
                break;
            }
            else {
                cout << "Unable to confirm intent.\n" << line << '\n';
                action = "noquit";
                continue;
            }
        } else if (action == "/pop") {
            pop = true;
            cout << "POP mode activated. Use '/drop' to return to dropping pieces.\n";
            continue;
        } else if (action == "/drop") {
            pop = false;
            cout << "DROP mode activated. Use '/pop' to switch to popping pieces.\n";
            continue;
        } else if (action == "1" || action == "2" || action == "3" || action == "4" || action == "5" || action == "6" || action == "7"){
            if (pop) {
                valid_move = pop_piece(stoi(action), symb);
            } else {
                valid_move = drop_piece(stoi(action), symb);
            }
            if (valid_move) {
                // after a valid move has been made, check if someone has won and update check_win accordingly
                status results = checkwin();
                if (results.winner_id == 3 || results.winner_id == next_player_id) { // you let your opponent win on your turn
                    display_board(board);
                    // send defeat message
                    cout << "Whoops! Player " << player_id << " let their opponent win!\n";
                    cout << "Player " << next_player_id << " wins! \n";
                    // add score to winner
                    scoreboard[next_player_id-1]++;
                    display_scoreboard();
                    break;
                } else if (results.checkwinner) {
                    display_board(board);
                    // send a victory message
                    cout << "Player " << results.winner_id << " wins! \n";
                    // add score to winner
                    scoreboard[results.winner_id-1]++;
                    display_scoreboard();
                    break;
                }
                //otherwise if nobody won
                round_num++; //only increase round number if a valid move was made
                pop = false; // reset pop condition only if a valid move has been made
                player_id = next_player_id; // switch to opponent as active player
            } else {
                continue;
            }
        } else {
            cout << "'" << action << "' is not a valid action.\n";
        }
        // check if anyone has won by reinitializing check_win
    } while (!check_win);
    
    // clear the board upon finishing the game
    clear_board();
}

// data type that the cpu uses
// col = 0 means no move
struct game_move {
    int col;
    bool pop;
};

//see if there is an easy vertical win
game_move vertical_win(string symbol) {
    const int start_row_index = 3;
    int row_index = 0;
    game_move action;
    action.col = 0;
    action.pop = false; // you can never win or prevent a loss by popping in vertical case
    for (const auto& row : board) {
        if (row_index < start_row_index) {
            row_index++;
            continue;
        }
        int col_index = 0;
        for (const auto& element : row) {
            int in_a_col = 1;
            if (element == symbol) {
                while (board[row_index - in_a_col][col_index] == symbol) {
                    in_a_col++;
                }
                if (in_a_col > 2 && board[row_index - in_a_col][col_index] == box) {
                    action.col = col_index+1;
                    return action; // return immediately if a 3 in a col of wanted symbol is found
                } 
            }
            col_index++;
        }
        row_index++;
    }
    return action;

}

//see if there is an easy hoerizontal win
game_move horizontal_dropwin(string symbol) {
    const int max_col_index = num_cols-3;
    int row_index = 0;
    game_move action;
    action.col = 0;
    action.pop = false; // dropwin function only allows dropping
    for (const auto& row : board) {
        int col_index = 0;
        for (const auto& element : row) {
            if (col_index == max_col_index) {
                break;
            }
            
            int sym_counter = 0;
            int box_counter = 0;
            int action_col_idx = 0;
            //check the 4 cells to the right
            if (element == symbol){
                sym_counter++;
            } else if (element == box) {
                box_counter++;
                action_col_idx = col_index+1;
            }
            if (board[row_index][col_index+1] == symbol){
                sym_counter++;
            } else if (board[row_index][col_index+1] == box) {
                box_counter++;
                action_col_idx = col_index+2;
            }
            if (board[row_index][col_index+2] == symbol){
                sym_counter++;
            } else if (board[row_index][col_index+2] == box) {
                box_counter++;
                action_col_idx = col_index+3;
            }
            if (board[row_index][col_index+3] == symbol){
                sym_counter++;
            } else if (board[row_index][col_index+3] == box) {
                box_counter++;
                action_col_idx = col_index+4;
            }
            //cout << "row " << row_index << " col " << col_index << " symcount " << sym_counter << " boxcount " << box_counter << '\n';
            if (sym_counter == 3 && box_counter == 1) {
                if (row_index == num_rows-1) { // if on ground, just whack
                    action.col = action_col_idx;
                    // cout << "put on ground at col " << action_col_idx <<'\n';
                    return action;
                } else if (board[row_index+1][action_col_idx-1] != box) { // otherwise check if there is solid ground to place a block in the empty space
                    action.col = action_col_idx;
                    //cout << "drop at col " << action_col_idx <<'\n';
                    return action;
                } // if no ground to drop, do nothing
            }
            
            col_index++;
        }
        row_index++;
    }
    return action;
}

// direction = 1 means up-right diagonal, direction = -1 = upleft diagonal
game_move diagonal_dropwin(string symbol, int direction) {
    const int start_row_index = 3;
    const int max_col_idx = num_cols-3;
    const int start_col_index = 3;
    game_move action;
    action.col = 0;
    action.pop = false; // dropwin function only allows dropping
    if (direction != 1 && direction != -1) {
        cout << "Someone messed with the code, this this function is not built to handle just any integer!\n";
        return action;
    }
    int row_index = 0;
    for (const auto& row : board) {
        if (row_index < start_row_index) {
            row_index++;
            continue;
        }
        int col_index = 0;
        for (const auto& element : row) {
            if (direction > 0 && col_index == max_col_idx) {
                break;
            } else if (direction < 0 && col_index < start_col_index) {
                col_index++;
                continue;
            }
            int sym_counter = 0;
            int box_counter = 0;
            int action_col_idx = 0;
            int box_index;
            if (element == symbol){
                sym_counter++;
            } else if (element == box) {
                box_counter++;
                action_col_idx = col_index+1;
                box_index = row_index;
            } 
            if (board[row_index-1][col_index+(1*direction)] == symbol){
                sym_counter++;
            } else if (board[row_index-1][col_index+(1*direction)] == box) {
                box_counter++;
                action_col_idx = col_index+(1*direction)+1;
                box_index = row_index-1;
            }
            if (board[row_index-2][col_index+(2*direction)] == symbol){
                sym_counter++;
            } else if (board[row_index-2][col_index+(2*direction)] == box) {
                box_counter++;
                action_col_idx = col_index+(2*direction)+1;
                box_index = row_index-2;
            }
            if (board[row_index-3][col_index+(3*direction)] == symbol){
                sym_counter++;
            } else if (board[row_index-3][col_index+(3*direction)] == box) {
                box_counter++;
                action_col_idx = col_index+(3*direction)+1;
                box_index = row_index-3;
            }
            // cout << "row " << row_index << " col " << col_index << " symcount " << sym_counter << " boxcount " << box_counter << '\n';

            if (sym_counter == 3 && box_counter == 1) {
                if (box_index == num_rows-1) { // if on ground, just whack
                    action.col = action_col_idx;
                    // cout << "put on ground at col " << action_col_idx <<'\n';
                    return action;
                } else if (board[box_index+1][action_col_idx-1] != box) { // otherwise check if there is solid ground to place a block in the empty space
                    action.col = action_col_idx;
                    // cout << "drop at col " << action_col_idx <<'\n';
                    return action;
                } // if no ground to drop, do nothing
            }
            col_index++;
        }
        row_index++;
    }
    return action;
}

// tries to execute a given game_move. returns true if successful.
bool execute_move(game_move wanted_move, string symbol) {
    if (wanted_move.col > 0 && !wanted_move.pop) {
        return drop_piece(wanted_move.col, symbol);
    } else if (wanted_move.col > 0) {
        return pop_piece(wanted_move.col, symbol);
    } else {
        return false;
    }
}

// basic survival instinct of the cpu
// returns true if a move was made
// bool pop determines whether cpu0 can make pop moves
bool cpu0(bool pop, string symbol) {
    // algorithm that scans the current board state and sees if there are any no-brainer moves
    // aka 3 in a rows of either type
    // ALWAYS prioritize winning moves over losing ones
    string opp_symb = player_1;
    if (symbol == player_1) {
        opp_symb = player_2;
    }
    bool valid_move = false;
    // scan the board for easy wins
    //scan wins before losses
    //vertical scan
    game_move vert_win = vertical_win(symbol);
    if (vert_win.col > 0) {
        cout << "Vertical win by dropping at column " << vert_win.col << ".\n" << line << '\n';
        return execute_move(vert_win, symbol); // win NOW
    }

    game_move hori_dropwin = horizontal_dropwin(symbol);
    if (hori_dropwin.col > 0) {
        cout << "Horizontal win by dropping at column " << hori_dropwin.col << ".\n" << line << '\n';
        return execute_move(hori_dropwin, symbol); // win NOW
    }

    game_move ur_diag_dropwin = diagonal_dropwin(symbol, 1);
    if (ur_diag_dropwin.col > 0) {
        cout << "Up-right diagonal win by dropping at column " << ur_diag_dropwin.col << ".\n" << line << '\n';
        return execute_move(ur_diag_dropwin, symbol);
    }

    game_move ul_diag_dropwin = diagonal_dropwin(symbol, -1);
    if (ul_diag_dropwin.col > 0) {
        cout << "Up-left diagonal win by dropping at column " << ul_diag_dropwin.col << ".\n" << line << '\n';
        return execute_move(ul_diag_dropwin, symbol);
    }

    // run the pop scans only if pop moves are permitted
    // it is not possible to stop a pop-move victory by dropping so no point
    if (pop) {
        // run pop scans here
    }

    game_move vert_loss = vertical_win(opp_symb);
    if (vert_loss.col > 0) {
        //cout << "Prevent oppoent's vertical win by dropping at column " << vert_loss.col << ".\n";
        cout << "Make a move at column " << vert_loss.col << ".\n" << line << '\n';
        return execute_move(vert_loss, symbol); // stop loss NOW
    }

    game_move hori_droploss = horizontal_dropwin(opp_symb);
    if (hori_droploss.col > 0) {
        //cout << "Prevent opponent's horizontal win by dropping at column " << hori_droploss.col << ".\n";
        cout << "Make a move at column " << hori_droploss.col << ".\n" << line << '\n';
        return execute_move(hori_droploss, symbol); // stop loss NOW
    }

    game_move ur_diag_droploss = diagonal_dropwin(opp_symb, 1);
    if (ur_diag_droploss.col > 0) {
        //cout << "Prevent opponent's up-right diagonal win by dropping at column " << ur_diag_droploss.col << ".\n";
        cout << "Make a move at column " << ur_diag_droploss.col << ".\n" << line << '\n';
        return execute_move(ur_diag_droploss, symbol);
    }
    game_move ul_diag_droploss = diagonal_dropwin(opp_symb, -1);
    if (ul_diag_droploss.col > 0) {
        //cout << "Prevent opponent's up-left diagonal win by dropping at column " << ul_diag_droploss.col << ".\n";
        cout << "Make a move at column " << ul_diag_droploss.col << ".\n" << line << '\n';
        return execute_move(ul_diag_droploss, symbol);
    }

    // if no vertical opportunities, check for horizontal opportunities


    return false;
}
// brain of level 1 cpu
// makes the actual move
void cpu1(string symbol) {
    // level 1 cpu makes random moves, only plays basic defense and offense
        // will only try to win if a 3 in a row is present
        // will try to defend only if you have an imminent 3 in a row
        // will make random drop moves otherwise
        // cannot use 'POP' moves unless entire board is full
    game_move cpu1_move;
    cpu1_move.pop = false; // always drop by default, never pop
    if (board_full()) {
        cpu1_move.pop = true;
    }
    bool valid_move = false;
    bool survival_instinct = cpu0(cpu1_move.pop, symbol);
    if (!survival_instinct) {
        // make random drop move
        do {
            cpu1_move.col = rand_col(); // select a random column to affect
            if (cpu1_move.pop) {
                valid_move = pop_piece(cpu1_move.col, symbol);
            } else {
                valid_move = drop_piece(cpu1_move.col, symbol);
            }
        } while (!valid_move);
        //cout << "Make a random move at column " << cpu1_move.col << ".\n";
        cout << "Make a move at column " << cpu1_move.col << ".\n" << line << '\n';
    }        
}

// brain of level 2 cpu
void cpu2(string symbol) {
    // level 2 cpu follows a set gameplan/stragey
        // will snap up a win if a 3 in a row is present
        // will try to defend if you have an imminent 3 in a row
        // otherwise, will try to follow its strategy
        // able to use pop moves
    game_move cpu2_move;
    cpu2_move.pop = false; // by default, don't anyhow pop
    if (board_full()) {
        cpu2_move.pop = true;
    }
    bool valid_move = false;
    bool survival_instinct = cpu0(true, symbol); // allow popping in survival situations
    if (!survival_instinct) {
        // try to drop pieces down the middle
        valid_move = drop_piece(4, symbol);
        if (!valid_move) {
            // if cannot, then just do a random move
            do {
            cpu2_move.col = rand_col(); // select a random column to affect
            if (cpu2_move.pop) {
                valid_move = pop_piece(cpu2_move.col, symbol);
            } else {
                valid_move = drop_piece(cpu2_move.col, symbol);
            }
            } while (!valid_move);
            cout << "Make a move at column " << cpu2_move.col << ".\n" << line << '\n';

        }
            
        
    }

}
// brain of level 3 cpu
void cpu3() {
    
}

// makes a move (changes board)
// different difficulty levels will result in different decision making of cpu (strategy)
void cpu_move(string symbol) {
    if (cpu_difficulty == 1) {
        cpu1(symbol); // use level 1 gyatt to make a move
    } else if (cpu_difficulty == 2) {
        cpu2(symbol); // use level 2 gyatt to make a move
    }
}

void singleplayer_gameloop() {
    create_board();
    // roll a dice to decide who goes first
    bool player_1_goes_first = coinflip();
    cout << "Use /quit as your turn action to forfeit the current match and return to gamemode selection.\n";
    int player_id; // who is the current player?
    if (player_1_goes_first) {
        player_id = 1;
    } else {
        player_id = 2;
    }
    bool check_win = false;
    int round_num = 1; 
    string action;
    bool pop = false;
    do {
        string action;
        string symb = player_2;
        int next_player_id = 1; // opponent's id
        bool valid_move;
        if (player_id == 1){
            symb = player_1;
            next_player_id = 2;
        }

        string mode = "drop";
        if (pop) {
            mode = "pop";
        }

        turn_display(player_id, round_num);
        display_board(board);
        cout << "Player " << player_id << " please make your move.\nYour symbol is '" << symb <<"'.\n";
        // if it is the human player's turn
        if (player_id == 1) {
            cout << "You are currently in '" << mode << "' mode.\n" << line << '\n';
            getline(cin, action);
            if (action == "/quit") {
                string quitt;
                cout << "Are you sure you want to forfeit the current match?\nYour opponent will be awarded a win if you forfeit.\nEnter 'yes' if you really want to quit.\n";
                cout << line <<'\n';
                getline(cin, quitt);
                if (quitt == "yes") {
                    cout << "Player " << player_id << " forefeits!\n" << line << '\n';
                    scoreboard[next_player_id-1]++;
                    display_scoreboard();
                    break;
                }
                else {
                    cout << "Unable to confirm intent.\n" << line << '\n';
                    action = "noquit";
                    continue;
                }
            } else if (action == "/pop") {
                pop = true;
                cout << "POP mode activated. Use '/drop' to return to dropping pieces.\n";
                continue;
            } else if (action == "/drop") {
                pop = false;
                cout << "DROP mode activated. Use '/pop' to switch to popping pieces.\n";
                continue;
            } else if (action == "1" || action == "2" || action == "3" || action == "4" || action == "5" || action == "6" || action == "7"){
                if (pop) {
                    valid_move = pop_piece(stoi(action), symb);
                } else {
                    valid_move = drop_piece(stoi(action), symb);
                }
                if (valid_move) {
                    // after a valid move has been made, check if someone has won and update check_win accordingly
                    status results = checkwin();
                    if (results.winner_id == 3 || results.winner_id == next_player_id) { // you let your opponent win on your turn
                        display_board(board);
                        // send defeat message
                        cout << "Whoops! Player " << player_id << " let their opponent win!\n";
                        cout << "Player " << next_player_id << " wins! \n";
                        // add score to winner
                        scoreboard[next_player_id-1]++;
                        display_scoreboard();
                        break;
                    } else if (results.checkwinner) {
                        display_board(board);
                        // send a victory message
                        cout << "Player " << results.winner_id << " wins! \n";
                        // add score to winner
                        scoreboard[results.winner_id-1]++;
                        display_scoreboard();
                        break;
                    }
                    //otherwise if nobody won
                    round_num++; //only increase round number if a valid move was made
                    pop = false; // reset pop condition only if a valid move has been made
                    player_id = next_player_id; // switch to opponent as active player
                } else {
                    continue;
                }
            } else {
            cout << "'" << action << "' is not a valid action.\n";
            }
        } else {
            cout << line << '\n';
            cpu_move(symb);
            status results = checkwin();
            if (results.winner_id == 3 || results.winner_id == next_player_id) { // you let your opponent win on your turn
                display_board(board);
                // send defeat message
                cout << "Whoops! Player " << player_id << " let their opponent win!\n";
                cout << "Player " << next_player_id << " wins! \n";
                // add score to winner
                scoreboard[next_player_id-1]++;
                display_scoreboard();
                break;
            } else if (results.checkwinner) {
                display_board(board);
                // send a victory message
                cout << "Player " << results.winner_id << " wins! \n";
                // add score to winner
                scoreboard[results.winner_id-1]++;
                display_scoreboard();
                break;
            }
            round_num++;
            pop = false;
            player_id = next_player_id;
        }
    } while (!check_win);
    // clear the board upon finishing the game
    clear_board();

}

//handles the actual gameplay
void run_game() {
// ask (cout) if the player wants to play single player vs a cpu or 2 player vs a human
    string gamemode;
    do{
        cout << "Would you like to play in Singleplayer mode versus a cpu or in 2-player mode against a human?\n";
        cout << "Enter '1' for Singleplayer or '2' for 2 players.\n";
        cout << "Enter '/back' to return to the main menu. The /help command can be only be acessed from the main menu.\n" << line << '\n';
        getline(cin, gamemode);
        if (gamemode == "/back") {
            continue;
        }else if (gamemode == "1") {
            cout << "Singleplayer mode selected.\n" << line << '\n';
            singleplayer_gameloop();
        } else if (gamemode == "2") {
            cout << "2-player mode selected.\n" << line << '\n';
            two_player_gameloop();
        } else {
            cout << "'" << gamemode << "' is not a recognized command.\n" << line << '\n';
        }
    } while (gamemode != "/back");
// if 2 human players, run one type of game loop (call 2_player_gameloop function or something)
// for 2 human players, ask them to decide amongst themselves who wants to be player 1 and wants to go second

// if versus cpu, run another type of game loop which calls upon the cpu to make moves
// for difficulty 1, you are allwoed to choose which player you would like to be
// for all other difficulties, roll a dice to decide if the human player is player 1 or 2.

}

// interprets and executes commands
void command_centre(string cmd) {
// command interpretation
    if (cmd == "/help") { //help command
        cout << "List of implemented commands: \n";
        cout <<"/start ---- Starts up a new game of Connect4.\n"; // 2-player mode implemented , level 1 cpu implemented
        cout <<"/settings ---- Opens up the settings menu.\n"; // implemented
        cout <<"/exit ---- Exits the game and shuts down the program.\n"; // implemented
        cout <<"/controls ---- Displays the game controls for a match of Connect4.\n"; // implemented
        cout <<"/rules ---- Displays the rules and objectives of the game.\n"; // implemented
        cout <<"/scoreboard ---- Displays the number of wins each player has.\n"; // implemented

    } else if (cmd == "/exit") {
        close = true;
        cout << "\nExiting Connect4.\n";
    } else if (cmd == "/start") { // start the connect4 game
        // have some while loop here while the game is running
        run_game();
        // default will be to drop pieces in. to pop, players must specify /pop
        // typing /drop will cancel a pop
    } else if (cmd == "/settings") {
        settings(); //open the settings menu
    } else if (cmd == "/scoreboard") {
        display_scoreboard();
    } else if (cmd == "/rules") {
        display_rules();
    } else if (cmd == "/controls") {
        display_controls();
    } else if (cmd == "/smile") {
        smile();
    } else {
        cout << "\n'" << cmd << "'" << "is not recognized as a command.\n";
    }
    
}



// main() accepts user commands and can start up connect4 games
int main() {
    //char command[20]; //max of 20 characters in a command
    string command;
    string startup_msg = "\nWelcome to the Connect4 main menu! \nType /help for a list of commands or /start to start the game!\n";
    do {
        cout << line << startup_msg << line << '\n';
        getline(cin, command);
        //cin.ignore();
        cout << "command received: " << command <<'\n'; //echo the command back to user
        cout << line << '\n';
        command_centre(command); //activate command centre

        
    } while (!close);
    
    return 0;
}