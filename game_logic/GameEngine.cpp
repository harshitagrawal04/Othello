#include <iostream>
#include <unordered_map>
#include <ranges>
#include <tuple>
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

// check if selected cell is inside the board or not
bool GameEngine::InBoard(const int row, const int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

// get the flips associated with a move from move_map, return empty vector if move not found
vector<Move> GameEngine::GetFlipsMap(Move current_move) {
    if (const auto flips = move_map.find(current_move); flips != move_map.end()) {
        return flips->second;
    }
    return {};
}

// return the opponent of the current player
CellState GameEngine::GetOpponent() {
    CellState opponent = currentPlayer == CellState::Black ?
                         CellState::White : CellState::Black;
    return opponent;
}

// convert row index (int) to character for display (0 -> 'A', 1 -> 'B', etc.)
char GameEngine::IntToChar(int Int) {
    return static_cast<char>(Int+65);
}

// convert row character to index (A/a -> 0, B/b -> 1, etc.)
int GameEngine::CharToInt(char ch) {
    return tolower(static_cast<unsigned char>(ch)) - 'a';
}

// get a vector of all moves (keys) currently available in move_map
vector<Move> GameEngine::GetKeys() {
    vector<Move> keys;

    // iterate through the map and push all keys into vector
    for (const auto &key: move_map | views::keys) {
        keys.push_back(key);
    }
    return keys;
}





/* ---------------------------------------------------------------------------------------
                                        CONSTRUCTURE
   ---------------------------------------------------------------------------------------  */

//default construct
GameEngine::GameEngine() {
    currentPlayer = CellState::Black;  // Black always starts
    histories.clear();                  // clear move history
    ResetBoard();                       // initialize board
    move_map = GetLegalMoves();         // populate legal moves for first player
}

/* ---------------------------------------------------------------------------------------
                                          CORE FUNCTION
   ---------------------------------------------------------------------------------------  */

// reset the board to initial starting position
void GameEngine::ResetBoard() {

    // set all cells to empty
    for (auto & row : board) {
        for (auto & cell : row) {
            cell = CellState::Empty;
        }
    }

    // set the four central cells according to standard Othello rules
    board[3][3] = CellState::White;
    board[4][4] = CellState::White;
    board[3][4] = CellState::Black;
    board[4][3] = CellState::Black;
}

// reset the game
void GameEngine::Reset() {
    currentPlayer = CellState::Black;
    histories.clear();
    ResetBoard();
    move_map = GetLegalMoves();
}

// determine which disks would be flipped if a move is played
vector<Move> GameEngine::GetFlips(const Move move) {
    vector<Move> flips;
    const CellState opponent = GetOpponent();

    // skip if cell is already occupied
    if (board[move.row][move.col] != CellState::Empty) {
        return flips;
    }

    // check all 8 directions from the move
    for (auto& direction: DIRECTIONS) {
        int row = move.row + direction[0];
        int col = move.col + direction[1];
        vector<Move> temp; // store flips in current direction

        // first cell in this direction must belong to opponent
        if (InBoard(row, col)) {
            if (board[row][col] == opponent) {
                temp.push_back(Move(row, col));
                row += direction[0];
                col += direction[1];
            } else continue; // skip this direction if first cell not opponent
        } else continue; // skip if out of board

        // continue in this direction while opponent disks are found
        while (InBoard(row, col) && board[row][col] == opponent) {
            temp.push_back(Move(row, col));
            row += direction[0];
            col += direction[1];
        }

        // if a current player disk is found after opponent disks, flips are valid
        if (board[row][col] == currentPlayer &&
            InBoard(row, col) && !temp.empty()) {
            for (auto& x : temp) {
                flips.push_back(x);
            }
        }
    }
    return flips;
}

// compute all legal moves for the current player
unordered_map<Move, vector<Move>> GameEngine::GetLegalMoves() {
    unordered_map<Move, vector<Move>> legal_moves;

    // check each cell on the board
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            // get flips for this move
            if (vector<Move> flips = GetFlips({row, col}); !flips.empty()) {
                legal_moves[{row, col}] = flips; // store valid moves
            }
        }
    }
    return legal_moves;
}

// check if a move is valid
bool GameEngine::IsValidMove(Move move) {
    bool valid1 = InBoard(move.row, move.col);               // check bounds
    bool valid2 = (move_map.find(move) != move_map.end());   // check if in legal moves

    cout << "Checking move: row=" << move.row << " col=" << move.col << endl;

    if (!valid1) cout << "Move outside the board\n";
    else if (!valid2) cout << "Illegal move\n";

    return valid1 && valid2;
}

// apply a move and flip disks accordingly
void GameEngine::MakeMove(int row, int col) {

    // save current state in history for undo
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

    // perform the move
    vector<Move> flips = GetFlips({row, col});
    board[row][col] = currentPlayer;

    // flip opponent disks
    for (auto& [r, c] : flips) {
        board[r][c] = currentPlayer;
    }

    currentPlayer = GetOpponent();      // switch turn
    move_map = GetLegalMoves();         // update legal moves
}

// undo the last move
void GameEngine::UndoMove() {

    if (histories.empty()) return; // nothing to undo

    const unsigned long last_index = histories.size() - 1;

    // restore the board state from last history
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            board[r][c] = histories[last_index].board[r][c];
        }
    }

    currentPlayer = histories[last_index].player; // restore player
    histories.pop_back();                          // remove last history
    move_map = GetLegalMoves();                    // update legal moves
}

// count number of black and white disks
pair<int,int> GameEngine::CountDisk() {
    int black = 0;
    int white = 0;

    for (const auto & row : board) {
        for (const auto cell : row) {
            if (cell == CellState::Black) black++;
            else if (cell == CellState::White) white++;
        }
    }

    return make_pair(black, white);
}

// check if game has ended (no legal moves for both players)
bool GameEngine::GameEnd() {
    if (move_map.empty()) {                  // current player has no moves
        currentPlayer = GetOpponent();      // switch player
        move_map = GetLegalMoves();
        if (move_map.empty()) {             // opponent also has no moves
            return true;                    // game over
        }
    }
    return false;
}

/* ---------------------------------------------------------------------------------------
                                        FUNCTION FOR AI
   ---------------------------------------------------------------------------------------  */

// converting the 2d board to 1d state for the AI
vector<float> GameEngine::BoardToState() {
    vector<float> state(BOARD_SIZE * BOARD_SIZE);

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == CellState::Black)
                state[row * BOARD_SIZE + col] = 1.0f;
            else if (board[row][col] == CellState::White)
                state[row * BOARD_SIZE + col] = -1.0f;
            else if(board[row][col] == CellState::Empty)
                state[row * BOARD_SIZE + col] = 0.0f;
        }
    }
    return state;
}

// convert the legal moves map to int
unordered_map<int, vector<int>> GameEngine::LegalMoveMapToStateMap() {
    unordered_map<int, vector<int>> map;
    for (const auto& pair: move_map) {
        vector<int> temp;
        Move key = pair.first;
        vector<Move> value = pair.second;
        for (const auto move: value) {
            temp.push_back(move.row * BOARD_SIZE + move.col);
        }
        int temp_key = key.row * BOARD_SIZE + key.col;
        map[temp_key] = temp;
    }
    return map;
}

// converts the legal moves to int
vector<int> GameEngine::StateMapIndices() {
    vector<int> keys;
    vector<Move> MoveKeys = GetKeys();

    for (const auto& key: MoveKeys) {
        keys.push_back(key.row * BOARD_SIZE + key.col);
    }
    return keys;
}

// environment for the RL model
tuple<vector<float>, float, bool> GameEngine::Step(int actionIndex) {
    int row = actionIndex / BOARD_SIZE;
    int col = actionIndex % BOARD_SIZE;

    vector<Move> flips = GetFlips({row, col});
    MakeMove(row, col);

    vector<float> newState = BoardToState();
    bool done = GameEnd();

    auto reward = static_cast<float>(flips.size());
    return {newState, reward, done};
}

void GameEngine::UndoAI() {
    UndoMove();
    UndoMove();
}


/* ---------------------------------------------------------------------------------------
                                        DISPLAY FUNCTION
   ---------------------------------------------------------------------------------------  */

// display a single cell on board
void GameEngine::DisplayCell(const CellState cell) {
    if (cell == CellState::Empty) cout << ". ";
    else if (cell == CellState::Black) cout << "B ";
    else if (cell == CellState::White) cout << "W ";
}

// display all available moves for the current player
void GameEngine::DisplayMoves() {
    cout << "Displaying moves" << endl;
    auto keys = GetKeys();
    for (const auto& [row, col] : keys) {
        cout << "{" << IntToChar(row) << ", " << col << "}" << endl;
    }
}

// display the full board with row/column labels
void GameEngine::DisplayBoard() {
    cout << "  ";
    for (int col = 0; col < BOARD_SIZE; col++) cout << col << " ";
    cout << endl;

    for (int row = 0; row < BOARD_SIZE; row++) {
        cout << IntToChar(row) << " "; // row label
        for (int col = 0; col < BOARD_SIZE; col++) {
            DisplayCell(board[row][col]); // display cell
        }
        cout << endl;
    }
}

// display full move history
void GameEngine::DisplayHistory() {
    for (auto& history: histories) {
        cout << "player: ";
        DisplayCell(history.player);

        cout << "\nmove made: {" << IntToChar(history.move.row) << ", "
             << history.move.col << "}\n";

        cout << "disks flipped: ";
        for (const auto&[row, col]: history.flipped) {
            cout << "{" << IntToChar(row) << ", " << col << "}, ";
        }
        cout << endl;
    }
}

// display current player
void GameEngine::DisplayPlayer() {
    if (currentPlayer == CellState::Black) cout << "Black ";
    else if (currentPlayer == CellState::White) cout << "White ";
}
