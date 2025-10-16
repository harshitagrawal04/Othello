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
    // loading the font
    if (!font.openFromFile("/Users/apple/Documents/programing/projects/othello/assists/fonts/Brush_Script.ttf")) {
        cerr << "font not found" << endl;
    }
}

void DisplayEngine::HandleMouseInput(const auto* mb) {
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

            // handling end game condition
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

void DisplayEngine::HandleKeyBoardInput(const auto* KeyPressed) {
    if (KeyPressed->scancode == sf::Keyboard::Scancode::Escape) {
        MainWindow.close();
    }
    if (KeyPressed->scancode == sf::Keyboard::Scancode::R) {
        gameEngine.Reset();
        CurrentState = GameState::InGame;
    }
    if (KeyPressed->scancode == sf::Keyboard::Scancode::U) {
        if (agent == nullptr) {
            gameEngine.UndoMove();
        }
        else {
            gameEngine.UndoAI();
        }

        if (CurrentState != GameState::InGame) {
            CurrentState = GameState::InGame;
        }
    }
}

// drawing on the screen
void DisplayEngine::Render() {
    if (CurrentState == GameState::MainMenu) {
        // setting up the coordinate for the text to be displayed
        float text_X = button_X + CellSize/4.0f;        // x coordinate for all button is common
        float text_2P_Y = button_2P_Y + CellSize/4.0f;  // y coordinate for 2 player button
        float text_B_Y = button_B_Y + CellSize/4.0f;    // y coordinate for basic bot button
        float text_I_Y = button_I_Y + CellSize/4.0f;    // y coordinate for intermediate bot button
        float text_A_Y = button_A_Y + CellSize/4.0f;    // y coordinate for advance bot button

        // creating the rectangle shape for the button
        sf::RectangleShape button;
        button.setSize({CellSize * 2.0f + 10.0f, CellSize});
        button.setOrigin({CellSize, CellSize / 2.0f});

        // creating the text to be displayed
        sf::Text text(font);
        text.setCharacterSize(30); // size in pixels
        text.setFillColor(sf::Color{0,0,0});
        text.setOrigin({CellSize + 10, CellSize / 2.0f});

        // two player button
        button.setPosition({button_X, button_2P_Y});
        MainWindow.draw(button);
        text.setString("1. 2 Player");
        text.setPosition({text_X, text_2P_Y});
        MainWindow.draw(text);

        // basic bot button
        button.setPosition({button_X, button_B_Y});
        MainWindow.draw(button);
        text.setString("2. basic bot");
        text.setPosition({text_X, text_B_Y});
        MainWindow.draw(text);

        // intermediate bot button
        button.setPosition({button_X, button_I_Y});
        MainWindow.draw(button);
        text.setString("3. intermediate bot");
        text.setPosition({text_X, text_I_Y});
        MainWindow.draw(text);

        // advance bot button
        button.setPosition({button_X, button_A_Y});
        MainWindow.draw(button);
        text.setString("4. Advance bot");
        text.setPosition({text_X, text_A_Y});
        MainWindow.draw(text);
    }

    else if (CurrentState == GameState::InGame) {
        const CellState (&currentBoard)[BOARD_SIZE][BOARD_SIZE] = gameEngine.GetBoard();
        vector<Move> keys = gameEngine.GetKeys();

        // initial coordinate of the first cell
        const float initial_x = 10.0f;
        const float initial_y = 10.0f;

        // creating the rectangle for the cell
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

                // drawing the disk if present on the board
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

                // drawing the possible move player can make
                else {
                    for (const auto key: keys) {
                        if (key.row == i && key.col == j) {
                            disk.setFillColor(sf::Color(128, 128, 128));
                            disk.setPosition({PositionX, PositionY});
                            MainWindow.draw(disk);
                            break;
                        }
                    }
                }
                x1 += CellSize;
            }
            y1 += CellSize;
            x1 = initial_x;
        }
    }

    else if (CurrentState == GameState::GameOver) {

        // counting the number of disk for each player
        auto [black, white] = gameEngine.CountDisk();

        // creating the text to be displayed for who has won or was it draw
        string text_display;
        if (black >= white) text_display = "Black wins: " + to_string(black) + "\n";
        else if (white > black) text_display = "White wins: " + to_string(white) + "\n";
        else text_display = "Draw \n";

        // creating the text
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

        // handling any input
        while (const std::optional event = MainWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {  // close the window when cross is click
                MainWindow.close();
            }

            // handling keyboard input
            if (const auto* KeyPressed = event->getIf<sf::Event::KeyPressed>()) {
                HandleKeyBoardInput(KeyPressed);
            }

            // handling mouse input
            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                HandleMouseInput(mb);
            }

            // if the screen is resize it will not affect the gameboard size
            if (event->is<sf::Event::Resized>()) {
                sf::View view(sf::FloatRect({0.f,0.f}, sf::Vector2f(MainWindow.getSize())));
                MainWindow.setView(view);
            }
        }

        MainWindow.clear(sf::Color(10, 20, 10));

        // drawing everything on the screen
        Render();

        MainWindow.display();
    }
}