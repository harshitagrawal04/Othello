#include "DisplayEngine.h"

DisplayEngine::DisplayEngine()
    : MainWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Othello - SFML 3 test"),
      CellSize(100),
      DiskRadius(CellSize/2 - 5),
      WhiteScore(0),
      BlackScore(0),
      Row(0),
      Col(0)
{

}


void DisplayEngine::HandleInput(const auto* mb) {
    if (mb->button == sf::Mouse::Button::Left) {
        Col = static_cast<int>(mb->position.x / CellSize);
        Row = static_cast<int>(mb->position.y / CellSize);
    }

    // Validate and make move
    if (gameEngine.IsValidMove({Row,Col})) {
        gameEngine.MakeMove(Row, Col);
        cout << "move maked" << endl;
    }
}

void DisplayEngine::Render() {
    const CellState (&currentBoard)[BOARD_SIZE][BOARD_SIZE] = gameEngine.GetBoard();

    float initial_x = 10.0f, initial_y = 10.0f;
    sf::RectangleShape square;
    square.setSize({CellSize, CellSize});
    square.setFillColor(sf::Color(0,255,0));
    square.setOutlineThickness(5.0f);
    square.setOutlineColor(sf::Color(10, 20, 10));

    sf::CircleShape disk;
    disk.setRadius(DiskRadius);

    float x1 = initial_x, y1 = initial_y;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            square.setPosition({x1, y1});
            MainWindow.draw(square);

            float PositionX = x1 + (CellSize - (DiskRadius * 2))/2;
            float PositionY = y1 + (CellSize - (DiskRadius * 2))/2;
            if (currentBoard[i][j] == CellState::Black) {
                disk.setFillColor(sf::Color(0, 0, 0));
                disk.setPosition({PositionX, PositionY});
                MainWindow.draw(disk);
            }
            else if (currentBoard[i][j] == CellState::White) {
                disk.setFillColor(sf::Color(255, 255, 255));
                disk.setPosition({PositionX, PositionY});
                MainWindow.draw(disk);
            }
            x1 += CellSize;
        }
        y1 += CellSize;
        x1 = initial_x;
    }
}

void DisplayEngine::run() {

    while (MainWindow.isOpen()) {
        while (const std::optional event = MainWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                MainWindow.close();
            }
            if (const auto* KeyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (KeyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    MainWindow.close();
                }
            }
            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                HandleInput(mb);
                cout << Row << " " << Col << endl;
            }
            if (event->is<sf::Event::Resized>()) {
                sf::View view(sf::FloatRect({0.f,0.f}, sf::Vector2f(MainWindow.getSize())));
                MainWindow.setView(view);
            }
        }

        MainWindow.clear(sf::Color(10, 20, 10));

        Render();

        MainWindow.display();
    }
}