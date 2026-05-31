#include <iostream>
#include <unordered_map>
#include <ranges>
#include "GameEngine.h"
#include "data_types/DataTypes.h"
#include "data_types/CommonFunctions.h"

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

std::vector<Move> GameEngine::GetFlips(Move move) {
    return GetFlips(move, GetCurrentPlayer());
}

// determine which disks would be flipped if a move is played
std::vector<Move> GameEngine::GetFlips(Move move, CellState player) {
    std::vector<Move> flips;
    CellState opponent = GetOpponent(player);

    // skip if cell is already occupied
    if (board[move.row][move.col] != CellState::Empty) {
        return flips;
    }

    // check all 8 directions from the move
    for (auto& direction: DIRECTIONS) {
        int row = move.row + direction[0];
        int col = move.col + direction[1];
        std::vector<Move> temp; // store flips in current direction

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
        if (board[row][col] == player &&
            InBoard(row, col) && !temp.empty()) {
            for (auto& x : temp) {
                flips.push_back(x);
            }
            }
    }
    return flips;
}

// get the flips associated with a move from moveMap, return empty std::vector if move not found
std::vector<Move> GameEngine::GetFlipsMap(Move currentMove) {
    if (const auto flips = moveMap.find(currentMove); flips != moveMap.end()) {
        return flips->second;
    }
    return {};
}

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

/* ---------------------------------------------------------------------------------------
                                        CONSTRUCTURE
   ---------------------------------------------------------------------------------------  */

//default construct
GameEngine::GameEngine() {
    currentPlayer = CellState::Black;  // Black always starts
    histories.clear();                  // clear move history
    ResetBoard();                       // initialize board
    moveMap = GetLegalMoves();         // populate legal moves for first player
}

/* ---------------------------------------------------------------------------------------
                                          CORE FUNCTION
   ---------------------------------------------------------------------------------------  */

// reset the game
void GameEngine::Reset() {
    currentPlayer = CellState::Black;
    histories.clear();
    ResetBoard();
    moveMap = GetLegalMoves();
}

// compute all legal moves for the current player
std::unordered_map<Move, std::vector<Move>> GameEngine::GetLegalMoves() {
    std::unordered_map<Move, std::vector<Move>> legalMoves;

    // check each cell on the board
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            // get flips for this move
            if (std::vector<Move> flips = GetFlips({row, col}); !flips.empty()) {
                legalMoves[{row, col}] = flips; // store valid moves
            }
        }
    }
    return legalMoves;
}

// check if a move is valid
bool GameEngine::IsValidMove(Move move) {
    bool valid1 = InBoard(move.row, move.col);               // check bounds
    bool valid2 = (moveMap.find(move) != moveMap.end());   // check if in legal moves

    std::cout << "Checking move: row=" << move.row << " col=" << move.col << std::endl;

    if (!valid1) std::cout << "Move outside the board\n";
    else if (!valid2) std::cout << "Illegal move\n";

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
    std::vector<Move> flips = GetFlips({row, col});
    board[row][col] = currentPlayer;

    // flip opponent disks
    for (auto& [r, c] : flips) {
        board[r][c] = currentPlayer;
    }

    currentPlayer = GetOpponent(currentPlayer);      // switch turn
    moveMap = GetLegalMoves();         // update legal moves
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
    moveMap = GetLegalMoves();                    // update legal moves
}

// count number of black and white disks
std::pair<int,int> GameEngine::CountDisk() {
    int black = 0;
    int white = 0;

    for (const auto & row : board) {
        for (const auto cell : row) {
            if (cell == CellState::Black) black++;
            else if (cell == CellState::White) white++;
        }
    }

    return std::make_pair(black, white);
}

// check if game has ended (no legal moves for both players)
bool GameEngine::GameEnd() {
    if (moveMap.empty()) {                  // current player has no moves
        currentPlayer = GetOpponent(currentPlayer);      // switch player
        moveMap = GetLegalMoves();
        if (moveMap.empty()) {             // opponent also has no moves
            return true;                    // game over
        }
    }
    return false;
}

std::vector<Move> GameEngine::GetKeys() {
    std::vector<Move> keys;

    // iterate through the map and push all keys into std::vector
    for (const auto &key: moveMap | std::views::keys) {
        keys.push_back(key);
    }
    return keys;
}

/* ---------------------------------------------------------------------------------------
                                        FUNCTION FOR AI
   ---------------------------------------------------------------------------------------  */

void GameEngine::UndoAI() {
    UndoMove();
    UndoMove();
}

std::pair<int,int> GameEngine::FrontierCount() {
    int frontier_black = 0;
    int frontier_white = 0;

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if(board[row][col] != CellState::Empty) {
                for (auto& direction: DIRECTIONS) {
                    int r_pos = row + direction[0];
                    int c_pos = col + direction[1];
                    if (InBoard(r_pos, c_pos) && board[r_pos][c_pos] == CellState::Empty) {
                        if (board[row][col] == CellState::Black)
                            frontier_black++;
                        else
                            frontier_white++;
                        break;
                    }
                }
            }
        }
    }
    return std::make_pair(frontier_black, frontier_white);
}

std::pair<int, int> GameEngine::StableCount() {
    const int CORNERS[4][2] = { {0, 0}, {0, 7}, {7, 0}, {7, 7} };
    // diagonal, vertical, anti-diagonal, horizontal
    const int DIRECTIONS[4][2] = {{1,1}, {1,0}, {1,-1}, {0,1}};
    int STABLE[BOARD_SIZE][BOARD_SIZE] {};

    int stable_black = 0;
    int stable_white = 0;
    int new_stable = 0;

    // 1. Initialize corners
    for (auto& corner: CORNERS) {
        int cr = corner[0];
        int cc = corner[1];
        if (board[cr][cc] == CellState::Black) {
            stable_black++;
            STABLE[cr][cc] = 1;
        }
        else if (board[cr][cc] == CellState::White) {
            stable_white++;
            STABLE[cr][cc] = 1;
        }
    }

    // LAMBDA FUNCTION: Captures local variables by reference
    auto IsSideSafe = [&](int r, int c, CellState current_color) -> bool {
        return !InBoard(r, c) || (STABLE[r][c] == 1 && board[r][c] == current_color);
    };

    // 2. Propagate stability iteratively
    do {
        new_stable = 0;
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {

                // Must actually have a piece, and not already be marked stable
                if (board[row][col] == CellState::Empty || STABLE[row][col] == 1) {
                    continue;
                }

                int stable_axes = 0;
                CellState current_color = board[row][col];

                for (auto& direction: DIRECTIONS) {
                    // Check positive direction side
                    bool side1_safe = IsSideSafe(row + direction[0], col + direction[1], current_color);

                    // Check negative direction side correctly using subtraction
                    bool side2_safe = IsSideSafe(row - direction[0], col - direction[1], current_color);

                    // An axis is safe if it's secure from either side
                    if (side1_safe || side2_safe) {
                        stable_axes++;
                    }
                }

                // If secure across all 4 axes, it's stable
                if (stable_axes == 4) {
                    STABLE[row][col] = 1;

                    if (board[row][col] == CellState::Black)
                        stable_black++;
                    else if (board[row][col] == CellState::White)
                        stable_white++;

                    new_stable++;
                }
            }
        }
    } while (new_stable > 0);

    return std::make_pair(stable_black, stable_white);
}

std::pair<int,int> GameEngine::CountLegalMove() {
    int legalMoves_self = 0;

    // check each cell on the board
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            // get flips for this move
            if (std::vector<Move> flips = GetFlips({row, col}); !flips.empty()) {
                legalMoves_self++; // store valid moves
            }
        }
    }

    int legalMoves_opponent = 0;

    // check each cell on the board
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            // get flips for this move
            if (std::vector<Move> flips = GetFlips({row, col}, GetOpponent(currentPlayer)); !flips.empty()) {
                legalMoves_opponent++; // store valid moves
            }
        }
    }

    // first number is for black second is white
    if (currentPlayer == CellState::Black)
        return std::make_pair(legalMoves_self, legalMoves_opponent);
    return std::make_pair(legalMoves_opponent, legalMoves_self);
}