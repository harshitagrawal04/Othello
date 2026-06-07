#include "DisplayEngine.h"
#include "ai_bot/AI.h"
#include "data_types/CommonFunctions.h"
#include <string>

/* ---------------------------------------------------------------------------------------
                                        CONSTRUCTURE
   ---------------------------------------------------------------------------------------  */

DisplayEngine::DisplayEngine():
    mainWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Othello - SFML 3 test"),
    currentState(GameState::MainMenu),
    cellSize(100),
    diskRadius(cellSize/2 - 5),
    row(0),
    col(0),
    player1(CellState::Black),
    player2(CellState::White)
{
    // loading the font
    if (!font.openFromFile("assets/fonts/Kaushan_Script/KaushanScript-Regular.ttf")) {
        std::cerr << "font not found" << std::endl;
    }
}

/* ---------------------------------------------------------------------------------------
                                        DISPLAY FUNCTION
   ---------------------------------------------------------------------------------------  */

void DisplayEngine::HandleMouseInput(const sf::Event::MouseButtonPressed* mb) {
    if (mb->button == sf::Mouse::Button::Left) {
        col = mb->position.x;
        row = mb->position.y;

        if (currentState == GameState::MainMenu) {
            if (col >= button_X - cellSize && col <= button_X + cellSize) {
                // two player
                if (row <= button_2P_Y + cellSize/2.0f && row >= button_2P_Y - cellSize/2.0f) {
                    currentState = GameState::InGame;
                    agent = CreateAIAgent("");
                    std::cout << "Agent: human";
                }

                // basic bot
                else if (row <= button_B_Y + cellSize/2.0f && row >= button_B_Y - cellSize/2.0f) {
                    currentState = GameState::InGame;
                    agent = CreateAIAgent("basic");
                    std::tie(player1, player2) = GetCellState();
                    std::cout << "Agent: basic";
                }

                // intermediate bot
                else if (row <= button_I_Y + cellSize/2.0f && row >= button_I_Y - cellSize/2.0f) {
                    currentState = GameState::InGame;
                    agent = CreateAIAgent("intermediate");
                    std::tie(player1, player2) = GetCellState();
                    std::cout << "Agent: intermediate";
                }

                // advance bot
                else if (row <= button_A_Y + cellSize/2.0f && row >= button_A_Y - cellSize/2.0f) {
                    currentState = GameState::InGame;
                    agent = CreateAIAgent("advance");
                    std::tie(player1, player2) = GetCellState();
                    std::cout << "Agent: advance";
                }
            }
        }

        else if (currentState == GameState::InGame) {

            // converting the mouse input to the coordinate of the board (8X8)
            col = static_cast<int>(mb->position.x / cellSize);
            row = static_cast<int>(mb->position.y / cellSize);
            std::cout << row << " " << col << std::endl;

            CellState currentPlayer = gameEngine.GetCurrentPlayer();

            // move make by player
            if (agent == nullptr || currentPlayer == player1) {
                // Validate and make move
                if (gameEngine.IsValidMove({row,col})) {
                    gameEngine.MakeMove(row, col);
                    std::cout << "move maked" << std::endl;
                }
            }
            else { // move make by AI
                Move move = agent->SelectMove(gameEngine, gameEngine.GetCurrentPlayer());

                // Validate and make move
                if (gameEngine.IsValidMove({move.row,move.col})) {
                    gameEngine.MakeMove(move.row, move.col);
                    std::cout << "move maked" << std::endl;
                }
            }

            // handling end game condition
            if (gameEngine.GameEnd()) {
                currentState = GameState::GameOver;
                std::cout << "game end" << std::endl;
            }
        }
    }
}

void DisplayEngine::HandleKeyBoardInput(const sf::Event::KeyPressed* keyPressed) {
    if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
        gameEngine.Reset();
        currentState = GameState::InGame;
    }
    if (keyPressed->scancode == sf::Keyboard::Scancode::U) {
        // player can only undo if the game is going on
        if (currentState == GameState::InGame) {
            if (agent == nullptr) {
                gameEngine.UndoMove();
            }
            else {
                gameEngine.UndoAI();
            }
        }
    }
    if (keyPressed->scancode == sf::Keyboard::Scancode::M) {
        gameEngine.Reset();
        currentState = GameState::MainMenu;
    }
    if (keyPressed->scancode == sf::Keyboard::Scancode::Q) {
        mainWindow.close();
    }
}

// drawing on the screen
void DisplayEngine::Render() {
    if (currentState == GameState::MainMenu) {
        // setting up the coordinate for the text to be displayed
        float text_X = button_X + cellSize/4.0f;        // x coordinate for all button is common
        float text_2P_Y = button_2P_Y + cellSize/4.0f;  // y coordinate for 2 player button
        float text_B_Y = button_B_Y + cellSize/4.0f;    // y coordinate for basic bot button
        float text_I_Y = button_I_Y + cellSize/4.0f;    // y coordinate for intermediate bot button
        float text_A_Y = button_A_Y + cellSize/4.0f;    // y coordinate for advance bot button

        // creating the rectangle shape for the button
        sf::RectangleShape button;
        button.setSize({cellSize * 2.5f + 10.0f, cellSize});
        button.setOrigin({cellSize, cellSize / 2.0f});

        // creating the text to be displayed
        sf::Text text(font);
        text.setCharacterSize(30); // size in pixels
        text.setFillColor(sf::Color{0,0,0});
        text.setOrigin({cellSize + 10, cellSize / 2.0f});

        // two player button
        button.setPosition({button_X, button_2P_Y});
        mainWindow.draw(button);
        text.setString("1. 2 Player");
        text.setPosition({text_X, text_2P_Y});
        mainWindow.draw(text);

        // basic bot button
        button.setPosition({button_X, button_B_Y});
        mainWindow.draw(button);
        text.setString("2. basic bot");
        text.setPosition({text_X, text_B_Y});
        mainWindow.draw(text);

        // intermediate bot button
        button.setPosition({button_X, button_I_Y});
        mainWindow.draw(button);
        text.setString("3. intermediate bot");
        text.setPosition({text_X, text_I_Y});
        mainWindow.draw(text);

        // advance bot button
        button.setPosition({button_X, button_A_Y});
        mainWindow.draw(button);
        text.setString("4. Advance bot");
        text.setPosition({text_X, text_A_Y});
        mainWindow.draw(text);
    }

    else if (currentState == GameState::InGame) {
        const CellState (&currentBoard)[BOARD_SIZE][BOARD_SIZE] = gameEngine.GetBoard();
        std::vector<Move> keys = gameEngine.GetKeys();

        // initial coordinate of the first cell
        const float initial_x = 10.0f;
        const float initial_y = 10.0f;
        const float offset_disk = -2.5f;

        // creating the rectangle for the cell
        sf::RectangleShape cell;
        cell.setSize({cellSize, cellSize});
        cell.setFillColor(sf::Color(0,255,0));
        cell.setOutlineThickness(5.0f);
        cell.setOutlineColor(sf::Color(0, 0, 0));

        sf::CircleShape disk;
        disk.setRadius(diskRadius);

        float x1 = initial_x, y1 = initial_y;

        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {

                // drawing the cell
                cell.setPosition({x1, y1});
                mainWindow.draw(cell);

                // setting the coordinates for the disk to be drawn
                float PositionX = x1 + (cellSize - (diskRadius * 2))/2 + offset_disk;
                float PositionY = y1 + (cellSize - (diskRadius * 2))/2 + offset_disk;

                // drawing the disk if present on the board
                if (currentBoard[i][j] == CellState::Black) {
                    disk.setFillColor(sf::Color(0, 0, 0));
                    disk.setPosition({PositionX, PositionY});
                    mainWindow.draw(disk);
                }
                else if (currentBoard[i][j] == CellState::White) {
                    disk.setFillColor(sf::Color(255, 255, 255));
                    disk.setPosition({PositionX, PositionY});
                    mainWindow.draw(disk);
                }

                // drawing the possible move player can make
                else {
                    for (const auto [row, col]: keys) {
                        if (row == i && col == j) {
                            disk.setFillColor(sf::Color(128, 128, 128));
                            disk.setPosition({PositionX, PositionY});
                            mainWindow.draw(disk);
                            break;
                        }
                    }
                }
                x1 += cellSize;
            }
            y1 += cellSize;
            x1 = initial_x;
        }
    }

    else if (currentState == GameState::GameOver) {

        // counting the number of disk for each player
        auto [black, white] = gameEngine.CountDisk();

        // creating the text to be displayed for who has won or was it draw
        std::string text_display;
        if (black >= white) text_display = "Black wins: " + std::to_string(black) + "\n";
        else if (white > black) text_display = "White wins: " + std::to_string(white) + "\n";
        else text_display = "Draw \n";

        // creating the text
        sf::Text text(font);
        text.setCharacterSize(30); // size in pixels
        text.setFillColor(sf::Color::White);
        text.setOrigin({cellSize + 10, cellSize / 2.0f});

        // printing the number of disk white have
        text.setPosition({button_X, button_2P_Y});
        text.setString("White: " + std::to_string(white) + "\n");
        mainWindow.draw(text);

        // printing the number of disk black have
        text.setPosition({button_X, button_B_Y});
        text.setString("Black: " + std::to_string(black) + "\n");
        mainWindow.draw(text);

        // print the wining colour
        text.setCharacterSize(60); // size in pixels
        text.setPosition({button_X, button_A_Y});
        text.setString(text_display);
        mainWindow.draw(text);
    }
}

/* ---------------------------------------------------------------------------------------
                                         DISPLAY RUN
   ---------------------------------------------------------------------------------------  */

void DisplayEngine::Run() {
    while (mainWindow.isOpen()) {

        // handling any input
        while (const std::optional event = mainWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {  // close the window when cross is click
                mainWindow.close();
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
                sf::View view(sf::FloatRect({0.f,0.f}, sf::Vector2f(mainWindow.getSize())));
                mainWindow.setView(view);
            }
        }

        mainWindow.clear(sf::Color(10, 20, 10));

        // drawing everything on the screen
        Render();

        mainWindow.display();
    }
}