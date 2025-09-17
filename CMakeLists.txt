cmake_minimum_required(VERSION 3.31)
project(othello)

set(CMAKE_CXX_STANDARD 20)

add_executable(othello
        main.cpp
        game_logic/GameEngine.cpp
        game_logic/GameEngine.h
        ai_bot/AiEngine.cpp
        ai_bot/AiEngine.h
        game_logic/Move.h
        game_logic/History.h
)



