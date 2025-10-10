#include <iostream>
#include <memory>
#include "game_logic/GameEngine.h"
#include "game_logic/Move.h"
#include "ai_bot/AI.h"
#include "ai_bot/basic/Greedy.h"
#include "ai_bot/advance/ActorCritic.h"
#include "ai_bot/intermediate/tabular.h"
#include "Display/DisplayEngine.h"
using namespace std;

/* --------------------------------------------------------------------------
      ADD LOGGING SYSTEM AND UNIT TEST
   -------------------------------------------------------------------------- */

void PlayGame(AI* ai = nullptr, CellState humanColor = CellState::Black) {
    GameEngine gameEngine;
    while (true) {

        // Display current state
        gameEngine.DisplayBoard();
        gameEngine.DisplayMoves();
        gameEngine.DisplayPlayer();

        CellState currentPlayer = gameEngine.GetCurrentPlayer();

        if (currentPlayer == humanColor || ai == nullptr) {
            // Take user input
            char char_row;
            int col;
            cout << "enter the move (row, col) or u for undo or r for reset: ";
            cin >> char_row;

            // handle undo
            if (char_row == 'u' || char_row == 'U') {
                if (ai == nullptr) gameEngine.UndoAI();
                else gameEngine.UndoMove();
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
            Move move = ai->SelectMove(gameEngine);

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

unique_ptr<AI> createAIAgent(const string& type) {
    string lowerType = type;
    // Convert to lowercase for case-insensitive comparison
    transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);

    if (lowerType == "basic bot" || lowerType == "2") {
        return make_unique<Greedy>();
    }
    if (lowerType == "intermediate bot" || lowerType == "3") {
        return make_unique<Tabular>();
    }
    if (lowerType == "advance bot" || lowerType == "4") {
        return make_unique<ActorCritic>();
    }
    return nullptr; // Return a null unique_ptr on failure
}


int main() {
    DisplayEngine display;
    display.run();

    // string userChoice;
    // cout << "enter the type of bot you want to play: \n"
    //      << "1. two player \n"
    //      << "2. basic bot \n"
    //      << "3. intermediate bot \n"
    //      << "4. advance bot \n";
    // cin >> userChoice;
    //
    // unique_ptr<AI> agent = createAIAgent(userChoice);
    //
    // PlayGame(agent.get(), CellState::Black); ;
    return 0;
}
