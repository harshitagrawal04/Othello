#include <iostream>
#include "game_logic/GameEngine.h"
#include "game_logic/Move.h"
#include "ai_bot/basic/Greedy.h"
using namespace std;

/* --------------------------------------------------------------------------
      ADD LOGGING SYSTEM AND UNIT TEST
   -------------------------------------------------------------------------- */



void PlayGameTwoPlayer() {
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

void PlayGameBasicBot() {
    GameEngine gameEngine;
    Greedy greedy;
    while (true) {

        // Display current state
        gameEngine.DisplayBoard();
        gameEngine.DisplayMoves();
        gameEngine.DisplayPlayer();

        if (gameEngine.GetCurrentPlayer() == CellState::Black) {
            // Take user input
            char char_row;
            int col;
            cout << "enter the move (row, col) or u for undo or r for reset: ";
            cin >> char_row;

            // handle undo
            if (char_row == 'u' || char_row == 'U') {
                gameEngine.UndoAI();
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
        }
        else {
            Move move = greedy.SelectMove(gameEngine);

            // Validate and make move
            if (gameEngine.IsValidMove({move.row,move.col})) {
                gameEngine.MakeMove(move.row, move.col);
                cout << "move maked" << endl;
            }

            cout << "Move maked: row: " << gameEngine.IntToChar(move.row)
                 << ", col: " << move.col << endl;
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
    string BotType;
    cout << "enter the type of bot you want to play: \n"
         << "1. two player \n"
         << "2. basic bot \n";
    cin >> BotType;
    if (BotType == "two player" || BotType == "1") PlayGameTwoPlayer();
    else if (BotType == "basic bot" || BotType == "2") PlayGameBasicBot();
    return 0;
}
