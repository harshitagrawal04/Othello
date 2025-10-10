
#ifndef DISPLAYENGINE_H
#define DISPLAYENGINE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../game_logic/GameEngine.h"
#include "../game_logic/History.h"
#include "../game_logic/Move.h"

using namespace std;

#define WINDOW_HEIGHT 1024
#define WINDOW_WIDTH 768


class DisplayEngine {
    GameEngine gameEngine;
    sf::RenderWindow MainWindow;
    float CellSize;
    float DiskRadius;
    int WhiteScore;
    int BlackScore;
    int Row;
    int Col;

    void HandleInput(const auto* mb);
    void Render();

public:
    DisplayEngine();
    void run();
};



#endif //DISPLAYENGINE_H
