#include <iostream>
#include <vector>
#include "../include/gameBoard.h"

using namespace std;

int main() {
    CellState board[8][8];
    resetBoard(board);
    displayBoard(board);

    CellState currentPlayer = CellState::White;

    while (!findAllLegalMoves(board, CellState::White).empty() &&
           !findAllLegalMoves(board, CellState::Black).empty()) {
        vector<Move> moves = findAllLegalMoves(board, currentPlayer);

        if (moves.empty()) {
            currentPlayer = (currentPlayer == CellState::White)
             ? CellState::Black
             : CellState::White;
            continue;
        }

        cout << "Available moves: ";
        displayMoves(moves);

        int r, c;
        cout << "Enter your move (row and col): ";
        cin >> r >> c;

        Move move = {r, c};
        if (r < 0 || r >= 8 || c < 0 || c >= 8) {
            cout << "Out of bounds! Try again.\n";
        }
        else if (legalMove(move, board, currentPlayer)) {
            makeMove(move, board, currentPlayer);
            displayBoard(board);
            currentPlayer = (currentPlayer == CellState::White)
             ? CellState::Black
             : CellState::White;
        }
        else {
            cout << "Illegal move! Try again.\n";
        }
           }

    auto [white, black] = countDisk(board);
    cout << "Final Score -> Black: " << black << " White: " << white << endl;

    if (black > white) cout << "Black wins!\n";
    else if (black < white) cout << "White wins!\n";
    else cout << "Draw!\n";
}
