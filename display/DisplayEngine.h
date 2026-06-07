
#ifndef DISPLAYENGINE_H
#define DISPLAYENGINE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "game_logic/GameEngine.h"
#include "ai_bot/AI.h"

#define WINDOW_HEIGHT 1024
#define WINDOW_WIDTH 820


class DisplayEngine {
    // =================== GAME CORE & STATES ===================
    GameEngine gameEngine;
    GameState currentState;
    int row;
    int col;
    std::unique_ptr<AI> agent;
    CellState player1;
    CellState player2; // second player or bot


    // =================== WINDOW & RENDERING ===================
    const float cellSize;
    const float diskRadius;
    sf::RenderWindow mainWindow;
    sf::Font font;

    // =================== MENU BUTTON LAYOUT ===================
    const float distance_button = cellSize / 4;
    const float button_X = WINDOW_WIDTH / 2.0f;
    const float button_2P_Y = (WINDOW_HEIGHT - (cellSize * 4 + (cellSize / 4) * 3)) / 2;
    const float button_B_Y = button_2P_Y + cellSize + distance_button;
    const float button_I_Y = button_B_Y + cellSize + distance_button;
    const float button_A_Y = button_I_Y + cellSize + distance_button;

    // =================== DISPLAY FUNCTIONS ===================
    void HandleMouseInput(const sf::Event::MouseButtonPressed* mb);
    void HandleKeyBoardInput(const sf::Event::KeyPressed* keyPressed);
    void Render();


public:
    // =================== CONSTRUCTOR ===================
    DisplayEngine();

    // =================== DISPLAY RUN ===================
    void Run();
};



#endif //DISPLAYENGINE_H
