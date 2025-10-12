
#ifndef DISPLAYENGINE_H
#define DISPLAYENGINE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "../game_logic/GameEngine.h"
#include "../ai_bot/AI.h"

using namespace std;

#define WINDOW_HEIGHT 1024
#define WINDOW_WIDTH 820

enum class GameState {
    MainMenu,  // Home screen: select 1-player or 2-player
    InGame,    // Currently playing Othello on the board
    GameOver   // Displaying final scores
};


class DisplayEngine {
    GameEngine gameEngine;
    sf::RenderWindow MainWindow;
    GameState CurrentState;
    const float CellSize;
    const float DiskRadius;
    int WhiteScore;
    int BlackScore;
    int Row;
    int Col;
    sf::Font font;
    unique_ptr<AI> agent;

    const float distance_button = CellSize / 4;
    const float button_X = WINDOW_WIDTH / 2.0f;
    const float button_2P_Y = (WINDOW_HEIGHT - (CellSize * 4 + (CellSize / 4) * 3)) / 2;
    const float button_B_Y = button_2P_Y + CellSize + distance_button;
    const float button_I_Y = button_B_Y + CellSize + distance_button;
    const float button_A_Y = button_I_Y + CellSize + distance_button;

public:
    DisplayEngine();
    void HandleInput(const auto* mb);
    void Render();
    void run();
};



#endif //DISPLAYENGINE_H
