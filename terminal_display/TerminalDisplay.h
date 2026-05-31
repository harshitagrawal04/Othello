#ifndef TERMINALDISPLAY_H
#define TERMINALDISPLAY_H


#include "data_types/DataTypes.h"
#include "game_logic/GameEngine.h"
#include "ai_bot/AI.h"


class TerminalDisplay {
    // =================== VARIABLES ===================
    GameEngine game;
    GameState currentState;
    std::unique_ptr<AI> agent;
    CellState player1;
    CellState player2; // second player or bot

    // =================== HELPER FUNCTIONS ===================
    int CharToInt(char ch);
    char IntToChar(int num);

    // =================== RENDERING HELPERS ===================
    void DisplayCell(CellState cell);
    void DisplayBoard();
    void DisplayMoves();
    void DisplayHistory();
    void DisplayPlayer();

    // ======================= VIEW SCENES ================================
    void Menu();
    void GamePlay();

public:
    // =================== CONSTRUCTOR ===================
    TerminalDisplay();

    // =================== DISPLAY RUN ===================
    void Run();
};



#endif //TERMINALDISPLAY_H
