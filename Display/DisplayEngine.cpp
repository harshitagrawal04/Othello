#include "DisplayEngine.h"
#include "../ai_bot/AI.h"
#include <string>

DisplayEngine::DisplayEngine()
    : MainWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Othello - SFML 3 test"),
      CurrentState(GameState::MainMenu),
      CellSize(100),
      DiskRadius(CellSize/2 - 5),
      WhiteScore(0),
      BlackScore(0),
      Row(0),
      Col(0)
{
    if (!font.openFromFile("/Users/apple/Documents/programing/porjects/othello/assists/fonts/Brush_Script.ttf")) {
        cerr << "font not found" << endl;
    }
}


void DisplayEngine::HandleInput(const auto* mb) {
    if (mb->button == sf::Mouse::Button::Left) {
        Col = mb->position.x;
        Row = mb->position.y;

        if (CurrentState == GameState::MainMenu) {
            if (Col >= button_X - CellSize && Col <= button_X + CellSize) {
                // two player
                if (Row <= button_2P_Y + CellSize/2.0f && Row >= button_2P_Y - CellSize/2.0f) {
                    CurrentState = GameState::InGame;
                    agent = createAIAgent("");
                    cout << "agent: human";
                }
                // basic bot
                else if (Row <= button_B_Y + CellSize/2.0f && Row >= button_B_Y - CellSize/2.0f) {
                    CurrentState = GameState::InGame;
                    agent = createAIAgent("basic");
                    cout << "agent: basic";
                }
                // intermediate bot
                else if (Row <= button_I_Y + CellSize/2.0f && Row >= button_I_Y - CellSize/2.0f) {
                    CurrentState = GameState::InGame;
                    agent = createAIAgent("intermediate");
                    cout << "agent: intermediate";
                }
                // advance bot
                else if (Row <= button_A_Y + CellSize/2.0f && Row >= button_A_Y - CellSize/2.0f) {
                    CurrentState = GameState::InGame;
                    agent = createAIAgent("advance");\
                    cout << "agent: advance";
                }
            }
        }

        else if (CurrentState == GameState::InGame) {
            Col = static_cast<int>(mb->position.x / CellSize);
            Row = static_cast<int>(mb->position.y / CellSize);
            cout << Row << " " << Col << endl;

            CellState currentPlayer = gameEngine.GetCurrentPlayer();

            if (currentPlayer == CellState::Black || agent == nullptr) {
                // Validate and make move
                if (gameEngine.IsValidMove({Row,Col})) {
                    gameEngine.MakeMove(Row, Col);
                    cout << "move maked" << endl;
                }
            }
            else {
                Move move = agent->SelectMove(gameEngine);

                // Validate and make move
                if (gameEngine.IsValidMove({move.row,move.col})) {
                    gameEngine.MakeMove(move.row, move.col);
                    cout << "move maked" << endl;
                }
            }

            if (gameEngine.GameEnd()) {
                CurrentState = GameState::GameOver;
                cout << "game end" << endl;
            }
        }

        else if (CurrentState == GameState::GameOver) {
            gameEngine.DisplayHistory();
        }
    }
}

void DisplayEngine::Render() {
    if (CurrentState == GameState::MainMenu) {
        float text_X = button_X + CellSize/4.0f;
        float text_2P_Y = button_2P_Y + CellSize/4.0f;
        float text_B_Y = button_B_Y + CellSize/4.0f;
        float text_I_Y = button_I_Y + CellSize/4.0f;
        float text_A_Y = button_A_Y + CellSize/4.0f;

        sf::RectangleShape start_button;
        start_button.setSize({CellSize * 2.0f + 10.0f, CellSize});
        start_button.setOrigin({CellSize, CellSize / 2.0f});

        sf::Text text(font);
        text.setCharacterSize(30); // size in pixels
        text.setFillColor(sf::Color::Black);
        text.setOrigin({CellSize + 10, CellSize / 2.0f});

        // two player
        start_button.setPosition({button_X, button_2P_Y});
        MainWindow.draw(start_button);
        text.setString("1. 2 Player");
        text.setPosition({text_X, text_2P_Y});
        MainWindow.draw(text);

        // basic bot
        start_button.setPosition({button_X, button_B_Y});
        MainWindow.draw(start_button);
        text.setString("2. basic bot");
        text.setPosition({text_X, text_B_Y});
        MainWindow.draw(text);

        // intermediate bot
        start_button.setPosition({button_X, button_I_Y});
        MainWindow.draw(start_button);
        text.setString("3. intermediate bot");
        text.setPosition({text_X, text_I_Y});
        MainWindow.draw(text);

        // advance bot
        start_button.setPosition({button_X, button_A_Y});
        MainWindow.draw(start_button);
        text.setString("4. Advance bot");
        text.setPosition({text_X, text_A_Y});
        MainWindow.draw(text);
    }

    else if (CurrentState == GameState::InGame) {
        const CellState (&currentBoard)[BOARD_SIZE][BOARD_SIZE] = gameEngine.GetBoard();

        float initial_x = 10.0f, initial_y = 10.0f;
        sf::RectangleShape cell;
        cell.setSize({CellSize, CellSize});
        cell.setFillColor(sf::Color(0,255,0));
        cell.setOutlineThickness(5.0f);
        cell.setOutlineColor(sf::Color(0, 0, 0));

        sf::CircleShape disk;
        disk.setRadius(DiskRadius);

        float x1 = initial_x, y1 = initial_y;

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                cell.setPosition({x1, y1});
                MainWindow.draw(cell);

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

    else if (CurrentState == GameState::GameOver) {
        auto [black, white] = gameEngine.CountDisk();

        string text_display;
        if (black >= white) text_display = "Black wins: " + to_string(black) + "\n";
        else if (white > black) text_display = "White wins: " + to_string(white) + "\n";
        else text_display = "Draw \n";

        sf::Text text(font);
        text.setCharacterSize(30); // size in pixels
        text.setFillColor(sf::Color::White);
        text.setOrigin({CellSize + 10, CellSize / 2.0f});

        // printing the number of disk white have
        text.setPosition({button_X, button_2P_Y});
        text.setString("White: " + to_string(white) + "\n");
        MainWindow.draw(text);

        // printing the number of disk black have
        text.setPosition({button_X, button_B_Y});
        text.setString("Black: " + to_string(black) + "\n");
        MainWindow.draw(text);

        // print the wining colour
        text.setCharacterSize(60); // size in pixels
        text.setPosition({button_X, button_A_Y});
        text.setString(text_display);
        MainWindow.draw(text);
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