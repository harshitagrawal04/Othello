cmake_minimum_required(VERSION 3.31)
project(othello)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# creating a library for the UI
add_library(display STATIC
        Display/DisplayEngine.cpp
        Display/DisplayEngine.h
)
target_link_libraries(display PUBLIC SFML::Graphics SFML::Window SFML::System)


# creating a library for the Game Logic
add_library(GameLogic
        game_logic/GameEngine.cpp
        game_logic/GameEngine.h
)

# creating a library for the AI bot
add_library(AiBot STATIC
        ai_bot/advance/HeuristicMinimaxAlphabeta.cpp
        ai_bot/advance/HeuristicMinimaxAlphabeta.h
        ai_bot/basic/greedy.cpp
        ai_bot/basic/greedy.h
        ai_bot/ai.h
        ai_bot/SelectAIAgent.cpp
        ai_bot/intermediate/MinimaxAlphabeta.cpp
        ai_bot/intermediate/MinimaxAlphabeta.h
        ai_bot/MinimaxAlphabetaBase/MinimaxAlphabetaBase.cpp
        ai_bot/MinimaxAlphabetaBase/MinimaxAlphabetaBase.h
)


# Main game executable
add_executable(othello main.cpp)
target_link_libraries(othello PRIVATE
        GameLogic
        AiBot
        display
        SFML::Graphics
        SFML::Window
        SFML::System
)

add_custom_command(TARGET othello POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/assets"
)

# ensures headers (.h) in each directories are visible when compiling
target_include_directories(othello PRIVATE game_logic ai_bot display)

# Include Google Test source
add_subdirectory(lib/googletest)

add_executable(UnitTests
        test_Cases/phase1_test.cpp
)

target_link_libraries(UnitTests PRIVATE gtest_main GameLogic)
target_include_directories(UnitTests PRIVATE
        game_logic
)

enable_testing()
add_test(NAME AllTests COMMAND UnitTests)


