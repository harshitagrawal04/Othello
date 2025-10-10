cmake_minimum_required(VERSION 3.31)
project(othello)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add SFML subdirectory
add_subdirectory(lib/SFML)

# creating a library for the Game Logic
add_library(GameLogic
        game_logic/GameEngine.cpp
        game_logic/GameEngine.h
)

# creating a library for the AI bot
add_library(AiBot STATIC
        ai_bot/advance/ActorCritic.cpp
        ai_bot/advance/ActorCritic.h
        ai_bot/basic/greedy.cpp
        ai_bot/basic/greedy.h
        ai_bot/ai.h
        ai_bot/intermediate/tabular.cpp
        ai_bot/intermediate/tabular.h
)

# creating a library for the UI
add_library(display STATIC
        Display/DisplayEngine.cpp
        Display/DisplayEngine.h
)
target_link_libraries(display sfml-graphics sfml-window sfml-system)


# Main game executable
add_executable(othello main.cpp)
target_link_libraries(othello GameLogic AiBot display sfml-graphics sfml-window sfml-system) #do not remove this

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


