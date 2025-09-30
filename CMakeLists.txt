cmake_minimum_required(VERSION 3.31)
project(othello)

set(CMAKE_CXX_STANDARD 20)

# creating a library for the Game Logic
add_library(GameLogic
        game_logic/GameEngine.cpp
        game_logic/GameEngine.h
)

# creating a library for the AI bot
add_library(AiBot STATIC
        ai_bot/advance/AiEngine.cpp
        ai_bot/advance/AiEngine.h
        ai_bot/basic/greedy.cpp
        ai_bot/basic/greedy.h
        ai_bot/ai.h
)

# creating a library for the UI
add_library(display STATIC
        Display/DisplayEngine.cpp
        Display/DisplayEngine.h
)


# Main game executable
add_executable(othello main.cpp)
target_link_libraries(othello GameLogic AiBot display) #do not remove this

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


