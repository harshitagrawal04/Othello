cmake_minimum_required(VERSION 3.31)
project(othello)

set(CMAKE_CXX_STANDARD 20)

add_executable(othello
        main.cpp
        game_logic/game_logic.cpp
        game_logic/game_logic.h
)



