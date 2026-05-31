#include <iostream>
#include "data_types/CommonFunctions.h"
#include "TerminalDisplay.h"


/* ---------------------------------------------------------------------------------------
                                     HELPER FUNCTION
   ---------------------------------------------------------------------------------------  */

// convert row index (int) to character for display (0 -> 'A', 1 -> 'B', etc.)
char TerminalDisplay::IntToChar(int num) {
    return static_cast<char>(num+65);
}

// convert row character to index (A/a -> 0, B/b -> 1, etc.)
int TerminalDisplay::CharToInt(char ch) {
    return tolower(static_cast<unsigned char>(ch)) - 'a';
}


/* ---------------------------------------------------------------------------------------
                                RENDERING HELPERS
   ---------------------------------------------------------------------------------------  */

// display a single cell on board
void TerminalDisplay::DisplayCell(const CellState cell) {
    if (cell == CellState::Empty) std::cout << ". ";
    else if (cell == CellState::Black) std::cout << "B ";
    else if (cell == CellState::White) std::cout << "W ";
}

// display all available moves for the current player
void TerminalDisplay::DisplayMoves() {
    std::cout << "Displaying moves" << std::endl;
    auto keys = game.GetKeys();
    for (const auto& [row, col] : keys) {
        std::cout << "{" << IntToChar(row) << ", " << col << "}" << std::endl;
    }
}

// display the full board with row/column labels
void TerminalDisplay::DisplayBoard() {
    const CellState (&board)[BOARD_SIZE][BOARD_SIZE] = game.GetBoard();
    std::cout << "  ";
    for (int col = 0; col < BOARD_SIZE; col++) std::cout << col << " ";
    std::cout << std::endl;

    for (int row = 0; row < BOARD_SIZE; row++) {
        std::cout << IntToChar(row) << " "; // row label
        for (int col = 0; col < BOARD_SIZE; col++) {
            DisplayCell(board[row][col]); // display cell
        }
        std::cout << std::endl;
    }
}

// display full move history
void TerminalDisplay::DisplayHistory() {
    const std::vector<History>& histories = game.GetHistories();
    for (auto& history: histories) {
        std::cout << "player: ";
        DisplayCell(history.player);

        std::cout << "\nmove made: {" << IntToChar(history.move.row) << ", "
             << history.move.col << "}\n";

        std::cout << "disks flipped: ";
        for (const auto&[row, col]: history.flipped) {
            std::cout << "{" << IntToChar(row) << ", " << col << "}, ";
        }
        std::cout << std::endl;
    }
}

// display current player
void TerminalDisplay::DisplayPlayer() {
    CellState currentPlayerState = game.GetCurrentPlayer();
    if (currentPlayerState == CellState::Black) std::cout << "Black \n";
    else if (currentPlayerState == CellState::White) std::cout << "White \n";
}

/* ---------------------------------------------------------------------------------------
                                         VIEW SCENES
   ---------------------------------------------------------------------------------------  */
void TerminalDisplay::Menu() {
    // add a way to throw a warning if the user enter some other values then 1-4

    std::cout << "1. 2 Player \n"
              << "2. basic bot \n"
              << "3. intermediate bot \n"
              << "4. advance bot \n"
              << "5. exit\n";

    int PlayerChoice;
    std::cin >> PlayerChoice;

    if (PlayerChoice == 1) {
        currentState = GameState::InGame;
        agent = CreateAIAgent("");
        std::cout << "agent: human";
    }
    else if (PlayerChoice == 2) {
        currentState = GameState::InGame;
        agent = CreateAIAgent("basic");
        std::tie(player1, player2) = GetCellState();
        std::cout << "agent: basic";
    }
    else if (PlayerChoice == 3) {
        currentState = GameState::InGame;
        agent = CreateAIAgent("intermediate");
        std::tie(player1, player2) = GetCellState();
        std::cout << "agent: intermediate";
    }
    else if (PlayerChoice == 4) {
        currentState = GameState::InGame;
        agent = CreateAIAgent("advance");
        std::tie(player1, player2) = GetCellState();
        std::cout << "agent: advance";
    }
    else if (PlayerChoice == 5) {
        currentState = GameState::GameOver;
    }
    else {
        std::cout << "Invalid Choice" << std::endl;
    }
}

void TerminalDisplay::GamePlay() {
    // Display current state
    DisplayBoard();
    DisplayPlayer();
    DisplayMoves();

    CellState current_player = game.GetCurrentPlayer();

    if (agent == nullptr || current_player == player1) {
        // Take user input
        char char_row;
        std::cout << "enter the move (row, col) \n"
             << "u for undo \n"
             << "r for reset \n"
             << "m for main menu\n"
             << "q for quit \n";

        std::cin >> char_row;

        // handle undo
        if (char_row == 'u' || char_row == 'U') {
            if (agent == nullptr) {
                game.UndoMove();
            }
            else {
                game.UndoAI();
            }
        }
        else if (char_row == 'r' || char_row == 'R') {
            game.Reset();
            return;
        }
        else if (char_row == 'm' || char_row == 'M') {
            game.Reset();
            currentState = GameState::MainMenu;
            return;
        }
        else if (char_row == 'q' || char_row == 'Q') {
            currentState = GameState::GameOver;
            return;
        }

        int col;
        std::cin >> col;
        int row = CharToInt(char_row);

        // Validate and make move
        if (game.IsValidMove({row,col})) {
            game.MakeMove(row, col);
            std::cout << "Move maked" << std::endl;
        }
        else {
            return;
        }

        // handle end game condition
        if (game.GameEnd()) {
            currentState = GameState::GameOver;
        }
    }
    else {
        Move move = agent->SelectMove(game, game.GetCurrentPlayer());

        // Validate and make move
        if (game.IsValidMove({move.row,move.col})) {
            game.MakeMove(move.row, move.col);
            std::cout << "Move maked" << std::endl;
        }

        // handle end game condition
        if (game.GameEnd()) {
            currentState = GameState::GameOver;
        }
    }
}

/* ---------------------------------------------------------------------------------------
                                        CONSTRUCTURE
   ---------------------------------------------------------------------------------------  */

TerminalDisplay::TerminalDisplay():
    currentState(GameState::MainMenu),
    player1(CellState::Black),
    player2(CellState::White) { }


/* ---------------------------------------------------------------------------------------
                                         DISPLAY RUN
   ---------------------------------------------------------------------------------------  */
void TerminalDisplay::Run() {
    while (true) {
        if (currentState == GameState::MainMenu) {
            Menu();
        }
        else if (currentState == GameState::InGame) {
            GamePlay();
        }
        else if (currentState == GameState::GameOver) {
            std::cout << "game Over" << std::endl;
            std::cout << "History" << std::endl;
            DisplayHistory();
            std::exit(0);
        }
    }
}
