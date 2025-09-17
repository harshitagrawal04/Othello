#include <iostream>
#include "game_logic/GameEngine.h"
#include "game_logic/Move.h"
using namespace std;

/* --------------------------------------------------------------------------
      ADD LOGGING SYSTEM AND UNIT TEST
      MAKE THE GAME ENGINE CLASS
   -------------------------------------------------------------------------- */

void displayVector(const vector<Move> &move, const int size) {
    for (int i = 0; i < size; i++) {
        cout << move[i].row << " " << move[i].col << " " << endl;
    }
}

void displayPlayer(CellState player) {
    if (player == CellState::Black) {
        cout << "Black ";
    }
    else if (player == CellState::White) {
        cout << "White ";
    }
}

int main() {
    GameEngine gameEngine;
    while (true) {
        gameEngine.DisplayBoard();
        gameEngine.DisplayMoves();

        char char_row; int col;
        displayPlayer(gameEngine.currentPlayer);
        cout << "enter the move (row, col) or u for undo: ";
        cin >> char_row;

        if (char_row == 'u' || char_row == 'U') {
            gameEngine.UndoMove();
            continue;
        }

        cin >> col;
        int row = gameEngine.CharToInt(char_row);


        if (gameEngine.IsValidMove({row,col})) {
            gameEngine.MakeMove(row, col);
            cout << "move maked" << endl;
        }

        if (gameEngine.move_map.empty()) {
            gameEngine.currentPlayer = gameEngine.GetOpponent();
            gameEngine.move_map = gameEngine.GetLegalMoves();
            if (gameEngine.move_map.empty()) {
                break;
            }
        }
    }

    gameEngine.DisplayHistory();

    auto [black, white] = gameEngine.CountDisk();
    if (black >= white) cout << "Black wins" << endl;
    else if (white > black) cout << "White wins" << endl;
    else cout << "Draw" << endl;

    return 0;
}


