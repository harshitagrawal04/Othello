//
// Created by Apple on 04/09/25.
//

#include "../include/gameBoard.h"

using namespace std;

// directions in which cells can flip
int directions[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    { 0, -1},          { 0, 1},
    { 1, -1}, { 1, 0}, { 1, 1}
};

char cellToChar(CellState cell) {
    if (cell == CellState::Empty) return '.';
    if (cell == CellState::Black) return 'B';
    return 'W';
}

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

void displayBoard(CellState board[8][8]) {
    cout << "  ";
    for (int col = 0; col < 8; col++) {
        cout << col << " ";
    }
    cout << endl;

    for (int row = 0; row < 8; row++) {
        cout << row << " ";
        for (int col = 0; col < 8; col++) {
            cout << cellToChar(board[row][col]) << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void displayMoves(const vector<Move>& moves) {
    for (auto& move : moves) {
        cout << "(" << move.row << "," << move.col << ")\n";
    }
    cout << endl;
}

bool legalMove(Move move, CellState board[8][8], CellState currentPlayer) {
    int row = move.row;
    int col = move.col;

    CellState opponent = (currentPlayer == CellState::White)
                     ? CellState::Black
                     : CellState::White;

    if (board[row][col] != CellState::Empty) {
        return false;
    }

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
            else continue;
        }
        else continue;

        while (newrow >= 0 && newrow < 8 && newcol >= 0 && newcol < 8) {
            if (board[newrow][newcol] == currentPlayer) {
                return true;
            }
            newrow += dRow;
            newcol += dCol;
        }
    }
    return false;
}

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

void makeMove(Move move, CellState board[8][8], CellState currentPlayer) {
    if (!legalMove(move, board, currentPlayer)) return;

    board[move.row][move.col] = currentPlayer;

    CellState opponent = (currentPlayer == CellState::White)
                 ? CellState::Black
                 : CellState::White;

    for (auto& dir : directions) {
        int dRow = dir[0];
        int dCol = dir[1];

        int newrow = move.row + dRow;
        int newcol = move.col + dCol;

        vector<pair<int, int>> toFlip;

        while (newrow >= 0 && newrow < 8 &&
               newcol >= 0 && newcol < 8 &&
               board[newrow][newcol] == opponent) {
            toFlip.push_back({newrow, newcol});
            newrow += dRow;
            newcol += dCol;
        }

        if (!toFlip.empty() &&
            newrow >= 0 && newrow < 8 &&
            newcol >= 0 && newcol < 8 &&
            board[newrow][newcol] == currentPlayer) {
            for (auto& cell : toFlip) {
                board[cell.first][cell.second] = currentPlayer;
            }
        }
    }
}

pair<int,int> countDisk(CellState board[8][8]) {
    int white = 0;
    int black = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == CellState::White) white++;
            else if (board[row][col] == CellState::Black) black++;
        }
    }
    return {white, black};
}
