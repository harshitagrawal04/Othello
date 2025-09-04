cmake_minimum_required(VERSION 3.31)
project(Othello)

set(CMAKE_CXX_STANDARD 20)

add_executable(Othello src/main.cpp
        src/gameBoard.cpp
        include/gameBoard.h)
