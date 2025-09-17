#include <iostream>
#include <unordered_map>
#include <ranges>
#include "GmaeEngine.h"
#include "Move.h"


using namespace std;

/* ---------------------------------------------------------------------------------------
                                        VARIABLE
   ---------------------------------------------------------------------------------------  */

// direction in which disks can flip
const int DIRECTIONS[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},  // top-left   , top   , top-right
    { 0, -1},          { 0, 1},    // left       ,         right
    { 1, -1}, { 1, 0}, { 1, 1}   // bottom-left, bottom, bottom-right
};



/* ---------------------------------------------------------------------------------------
                                     HELPER FUNCTION
   ---------------------------------------------------------------------------------------  */

// helper function to check if selected cell is inside the board or not
bool InBoard(const int row, const int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

// helper function to display the cell
void DisplayCell(const CellState cell) {
    if (cell == CellState::Empty) {
        cout << ". ";
    }
    else if (cell == CellState::Black) {
        cout << "B ";
    }
    else if (cell == CellState::White) {
        cout << "W ";
    }
}

// helper function to find the opponent
CellState GetOpponent(const CellState current_player) {
    const CellState opponent = current_player == CellState::Black ?
                         CellState::White : CellState::Black;
    return opponent;
}

// helper function to get the list of all the key in the map
vector<Move> GetKeys(const unordered_map<Move, vector<Move>>& moves) {
    vector<Move> keys;

    // loop through the map and add move coordinates to the keys vector
    for (const auto &key: moves | views::keys) {
        keys.push_back(key);
    }
    return keys;
}

// helper function to get the flips for the move
vector<Move> GetFlipsMap(const unordered_map<Move, vector<Move>>& moves, const Move current_move) {
    if (const auto flips = moves.find(current_move); flips != moves.end()) {
        return flips->second;
    }
    return {};
}

// helper function to convert the row int to char for printing
char IntToChar(int Int) {
    return static_cast<char>(Int+65);
}

// helper function to convert the char to int
int CharToInt(char ch) {
    char lower_char = tolower(ch);
    return lower_char - 'a';
}


/* ---------------------------------------------------------------------------------------
                                         MAIN FUNCTION
   ---------------------------------------------------------------------------------------  */

// resetting the board to beginning stage of the game
void ResetBoard(CellState board[BOARD_SIZE][BOARD_SIZE]) {

    // setting each cell to empty
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = CellState::Empty;
        }
    }

    // setting the value of central four cell
    board[3][3] = CellState::Black;
    board[4][4] = CellState::Black;
    board[3][4] = CellState::White;
    board[4][3] = CellState::White;
}

// find if the particular move is legal or not and return all the disk that will flip
vector<Move> GetFlips(CellState board[BOARD_SIZE][BOARD_SIZE],
                      const Move move, const CellState current_player) {
    vector<Move> flips;
    const CellState opponent = GetOpponent(current_player);


    // checking if the cell is empty if not then return empty vector
    if (board[move.row][move.col] != CellState::Empty) {
        return flips;
    }

    // checking all the direction
    for (auto& direction: DIRECTIONS) {
        /* cout << "direction: " <<direction << endl; */
        int newrow = move.row + direction[0];
        int newcol = move.col + direction[1];
        vector<Move> temp;

        // checking if the new cell is in the board
        if (InBoard(newrow, newcol)) {
            // checking if the immediate cell is of opponent
            if (board[newrow][newcol] == opponent) {
                temp.push_back(Move(newrow, newcol));
                /*cout << "immediate cell " << newrow << "," << newcol << endl;*/
                newrow += direction[0];
                newcol += direction[1];
            }
            else continue; // if not then skip this direction
        }
        else continue; // if outside the board then skip this direction

        // while we are in the board and next cell is of opponent continue in the direction
        while (InBoard(newrow, newcol) && board[newrow][newcol] == opponent) {
            // if not then append the temp move and go forward in the same direction
            temp.push_back(Move(newrow, newcol));
            /*cout << "next cell " << newrow << "," << newcol << endl;*/
            newrow += direction[0];
            newcol += direction[1];
        }

        // if we find current player disk in the direction then append all the disk to be changed to legal_move
        if (InBoard(newrow, newcol) &&
            board[newrow][newcol] == current_player &&
            !temp.empty()) {
            for (auto& x : temp) {
                flips.push_back(x);
            }
        }
    }
    return flips;
}

// get all the possible legal moves
unordered_map<Move, vector<Move>> GetLegalMoves(CellState board[BOARD_SIZE][BOARD_SIZE],
                                                const CellState current_player) {
    unordered_map<Move, vector<Move>> legal_moves;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (vector<Move> flips = GetFlips(board, {row, col}, current_player); !flips.empty()) {
                legal_moves[{row, col}] = flips;
            }
        }
    }
    return legal_moves;
}

// make the move and change the disk accordingly
void MakeMove(CellState board[BOARD_SIZE][BOARD_SIZE], const CellState current_player,
              const Move move, const vector<Move>& flips) {

    // checking if the move is inside the board or not
    if (!InBoard(move.row, move.col)) {
        cout << "Illegal move" << endl;
    }

    // set the cell on which player is making the move to their colour
    board[move.row][move.col] = current_player;

    // flip the remaining disk
    for (auto& [row, col] : flips) {
        board[row][col] = current_player;
    }
}

pair<int,int> CountDisk(CellState board[BOARD_SIZE][BOARD_SIZE]) {
    int black = 0;
    int white = 0;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == CellState::Black) {
                black++;
            }
            else if (board[row][col] == CellState::White) {
                white++;
            }
        }
    }

    return make_pair(black, white);
}

void UndoMove(CellState board[BOARD_SIZE][BOARD_SIZE], vector<History>& histories) {

    // if there is no move to undo then a safeguard to not get error
    if (histories.empty()) return;


    const unsigned long last_index = histories.size() - 1;

    const CellState opponent = GetOpponent(histories[last_index].player);
    auto [row, col] = histories[last_index].move;
    vector<Move> flipped = histories[last_index].flipped;

    // setting the cell in which player played their move to empty
    board[row][col] = CellState::Empty;

    // undoing the disk flip
    for (auto& [r, c] : flipped) {
        board[r][c] = opponent;
    }

    histories.pop_back();
}


/* ---------------------------------------------------------------------------------------
                                        DISPLAY FUNCTION
   ---------------------------------------------------------------------------------------  */



// display all the available moves for the current player
void DisplayMoves(const unordered_map<Move, vector<Move>>& move) {
    for (vector<Move> moves = GetKeys(move); const auto&[row, col] : moves) {
        cout << "{" << IntToChar(row) << ", " << col << "}" << endl;
    }
}

// display the board with row and column number
void DisplayBoard(CellState board[BOARD_SIZE][BOARD_SIZE]) {
    cout << "  ";

    // print column number
    for (int col = 0; col < BOARD_SIZE; col++) {
        cout << col << " ";
    }
    cout << endl;

    // displaying the row number
    for (int row = 0; row < BOARD_SIZE; row++) {
        // displaying alphabetic characters for the rows
        cout << IntToChar(row) << " ";
        // displaying the cell
        for (int col = 0; col < BOARD_SIZE; col++) {
            DisplayCell(board[row][col]);
        }
        cout << endl;
    }
}


void DisplayHistory(const History& history) {
    // displaying the player
    cout << "player: ";
    DisplayCell(history.player);
    cout << endl;

    // displaying the move made
    cout << "move made: ";
    cout << "{" << IntToChar(history.move.row) << ", " << history.move.col << "}" << endl;

    // displaying the disks flipped
    cout << "disks flipped: ";
    for (const auto&[row, col]: history.flipped) {
        cout << "{" << IntToChar(row) << ", " << col << "}, ";
    }
    cout << endl;

}