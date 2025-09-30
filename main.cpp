#include <iostream>
#include "game_logic/GameEngine.h"
#include "game_logic/Move.h"
using namespace std;

/* --------------------------------------------------------------------------
      ADD LOGGING SYSTEM AND UNIT TEST
      MAKE THE GAME ENGINE CLASS
   -------------------------------------------------------------------------- */




void PlayGame() {
    GameEngine gameEngine;
    while (true) {

        // Display current state
        gameEngine.DisplayBoard();
        gameEngine.DisplayMoves();
        gameEngine.DisplayPlayer();

        // Take user input
        char char_row;
        int col;
        cout << "enter the move (row, col) or u for undo or r for reset: ";
        cin >> char_row;

        // handle undo
        if (char_row == 'u' || char_row == 'U') {
            gameEngine.UndoMove();
            continue;
        }
        if (char_row == 'r' || char_row == 'R') {
            gameEngine.Reset();
            continue;
        }

        cin >> col;
        int row = gameEngine.CharToInt(char_row);

        // Validate and make move
        if (gameEngine.IsValidMove({row,col})) {
            gameEngine.MakeMove(row, col);
            cout << "move maked" << endl;
        }

        // handle end game condition
        if (gameEngine.GameEnd()) {
            break;
        }
    }

    // After game ends
    gameEngine.DisplayHistory();
    auto [black, white] = gameEngine.CountDisk();

    if (black >= white) cout << "Black wins: " << black << endl;
    else if (white > black) cout << "White wins: " << white << endl;
    else cout << "Draw" << endl;
}


int main() {
    PlayGame();
    return 0;
}
