#include <iostream>
#include <vector>

using namespace std;

// declaring the states in which each cell can be
enum class CellState {
    Empty,
    Black,
    White
};

// helper function to display the value of cell
char cellToChar(CellState cell) {
    if (cell == CellState::Empty) return '.';
    if (cell == CellState::Black) return 'B';
    return 'W';
}

// setting all the cells to empty
void resetBoard(CellState board[8][8]) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col] = CellState::Empty;
        }
    }
    board[3][3] = CellState::Black;
    board[4][4] = CellState::Black;
    board[3][4] = CellState::White;
    board[4][3] = CellState::White;
}

// display the board
void displayBoard(CellState board[8][8]) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            cout << cellToChar(board[row][col]) << " ";
        }
        cout << endl;
    }
}

struct Move {
    int row;
    int col;
};

// directions in which cells can flip
int directions[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},   // up-left, up, up-right
    { 0, -1},          { 0, 1},   // left,      right
    { 1, -1}, { 1, 0}, { 1, 1}    // down-left, down, down-right
};

// checking if the given move is legal or not
bool legalMove(Move move, CellState board[8][8], CellState currentPlayer) {
    int row = move.row;
    int col = move.col;

    CellState opponent = (currentPlayer == CellState::White)
                     ? CellState::Black
                     : CellState::White;


    if (board[row][col] != CellState::Empty) {
        return false;
    }
    else {
        for (auto& dir : directions) {
            int dRow = dir[0];
            int dCol = dir[1];

            int newrow = row + dRow;
            int newcol = col + dCol;

            if (newrow >= 0 && newrow < 8 && newcol >= 0 && newcol < 8) {
                if (board[newrow][newcol] == opponent) {
                    newrow += dRow;
                    newcol += dCol;
                }
                else {
                    continue;
                }
            }
            else {
                continue;
            }

            while (newrow >= 0 && newrow < 8 && newcol >= 0 && newcol < 8) {
                if (board[newrow][newcol] == currentPlayer) {
                    return true;
                }
                newrow += dRow;
                newcol += dCol;
            }
        }
    }
    return false;
}

// checking for all legal moves for the current player
vector<Move> findAllLegalMoves(CellState board[8][8], CellState currentPlayer) {
    vector<Move> moves;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (legalMove({row, col}, board, currentPlayer)) {
                moves.push_back({row, col});
            }
        }
    }

    return moves;
}

int main() {
    CellState board[8][8];

    resetBoard(board);

    displayBoard(board);

    vector<Move> moves;


    moves = findAllLegalMoves(board, CellState::White);

    for (auto& move : moves) {
        cout << move.row << ", " << move.col << endl;
    }




    return 0;
}