#include <iostream>
#include <unordered_map>
#include <ranges>
#include "GameEngine.h"
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
bool GameEngine::InBoard(const int row, const int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

// helper function to display the cell
void GameEngine::DisplayCell(const CellState cell) {
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
CellState GameEngine::GetOpponent() {
    CellState opponent = currentPlayer == CellState::Black ?
                         CellState::White : CellState::Black;
    return opponent;
}

// helper function to get the list of all the key in the map
vector<Move> GameEngine::GetKeys() {
    vector<Move> keys;

    // loop through the map and add move coordinates to the keys vector
    for (const auto &key: move_map | views::keys) {
        keys.push_back(key);
    }
    return keys;
}

// helper function to get the flips for the move
vector<Move> GameEngine::GetFlipsMap(Move current_move) {
    if (const auto flips = move_map.find(current_move); flips != move_map.end()) {
        return flips->second;
    }
    return {};
}

// helper function to convert the row int to char for printing
char GameEngine::IntToChar(int Int) {
    return static_cast<char>(Int+65);
}

// helper function to convert the char to int
int GameEngine::CharToInt(char ch) {
    return tolower(static_cast<unsigned char>(ch)) - 'a';
}


/* ---------------------------------------------------------------------------------------
                                        CONSTRUCTURE
   ---------------------------------------------------------------------------------------  */

//default construct
GameEngine::GameEngine() {
    currentPlayer = CellState::Black;
    histories.clear();
    ResetBoard();
    move_map = GetLegalMoves();
}

/* ---------------------------------------------------------------------------------------
                                         MAIN FUNCTION
   ---------------------------------------------------------------------------------------  */

// resetting the board to beginning stage of the game
void GameEngine::ResetBoard() {

    // setting each cell to empty
    // accessing a particular row of the board
    for (auto & row : board) {
        // accessing a particular cell of the row
        for (auto & cell : row) {
            cell = CellState::Empty;
        }
    }

    // setting the value of central four cell
    board[3][3] = CellState::White;
    board[4][4] = CellState::White;
    board[3][4] = CellState::Black;
    board[4][3] = CellState::Black;
}

// find if the particular move is legal or not and return all the disk that will flip
vector<Move> GameEngine::GetFlips(const Move move) {
    vector<Move> flips;
    const CellState opponent = GetOpponent();


    // checking if the cell is empty if not then return empty vector
    if (board[move.row][move.col] != CellState::Empty) {
        return flips;
    }

    // checking all the direction
    for (auto& direction: DIRECTIONS) {
        int row = move.row + direction[0];
        int col = move.col + direction[1];
        vector<Move> temp;

        // checking if the new cell is in the board
        if (InBoard(row, col)) {
            // checking if the immediate cell is of opponent
            if (board[row][col] == opponent) {
                temp.push_back(Move(row, col));
                row += direction[0];
                col += direction[1];
            }
            else continue; // if not then skip this direction
        }
        else continue; // if outside the board then skip this direction

        // while we are in the board and next cell is of opponent continue in the direction
        while (InBoard(row, col) && board[row][col] == opponent) {
            // if not then append the temp move and go forward in the same direction
            temp.push_back(Move(row, col));
            row += direction[0];
            col += direction[1];
        }

        // if we find current player disk in the direction then append all the disk to be changed to legal_move
        if (InBoard(row, col) &&
            board[row][col] == currentPlayer &&
            !temp.empty()) {
            for (auto& x : temp) {
                flips.push_back(x);
            }
        }
    }
    return flips;
}

// get all the possible legal moves
unordered_map<Move, vector<Move>> GameEngine::GetLegalMoves() {
    unordered_map<Move, vector<Move>> legal_moves;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (vector<Move> flips = GetFlips({row, col}); !flips.empty()) {
                legal_moves[{row, col}] = flips;
            }
        }
    }
    return legal_moves;
}

// checking is enter move is legal or not
bool GameEngine::IsValidMove(Move move) {
    bool valid1 = InBoard(move.row, move.col);
    bool valid2 = (move_map.find(move) != move_map.end());

    cout << "Checking move: row=" << move.row << " col=" << move.col << endl;

    if (!valid1) cout << "Move outside the board\n";
    else if (!valid2) cout << "Illegal move\n";

    return valid1 && valid2;
}

// make the move and change the disk accordingly
void GameEngine::MakeMove(int row, int col) {

    // updating the history
    History h;
    h.player = currentPlayer;
    h.move = {row, col};
    h.flipped = GetFlipsMap({row, col});

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            h.board[r][c] = board[r][c];
        }
    }

    histories.push_back(h);

    vector<Move> flips = GetFlips({row, col});

    // set the cell on which player is making the move to their colour
    board[row][col] = currentPlayer;

    // flip the remaining disk
    for (auto& [r, c] : flips) {
        board[r][c] = currentPlayer;
    }

    currentPlayer = GetOpponent();
    move_map = GetLegalMoves();
}

pair<int,int> GameEngine::CountDisk() {
    int black = 0;
    int white = 0;

    // accessing a particular row of the board
    for (const auto & row : board) {
        // accessing a particular cell of the row
        for (const auto cell : row) {
            if (cell == CellState::Black) {
                black++;
            }
            else if (cell == CellState::White) {
                white++;
            }
        }
    }

    return make_pair(black, white);
}

void GameEngine::UndoMove() {

    // if there is no move to undo then a safeguard to not get error
    if (histories.empty()) return;


    const unsigned long last_index = histories.size() - 1;

    vector<Move> flipped = histories[last_index].flipped;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            board[r][c] = histories[last_index].board[r][c];
        }
    }

    currentPlayer = histories[last_index].player;
    histories.pop_back();
    move_map = GetLegalMoves();
}


/* ---------------------------------------------------------------------------------------
                                        DISPLAY FUNCTION
   ---------------------------------------------------------------------------------------  */



// display all the available moves for the current player
void GameEngine::DisplayMoves() {
    cout << "Displaying moves" << endl;
    auto keys = GetKeys();
    for (const auto& [row, col] : keys) {
        cout << "{" << IntToChar(row) << ", " << col << "}" << endl;
    }
}

// display the board with row and column number
void GameEngine::DisplayBoard() {
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


void GameEngine::DisplayHistory() {
    // displaying the history
    for (auto& history: histories) {
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
}