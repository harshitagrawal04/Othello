//
// Created by Apple on 04/09/25.
//

#ifndef GAMEBOARD_H
#define GAMEBOARD_H


#include <iostream>
#include <vector>

enum class CellState {
    Empty,
    Black,
    White
};

struct Move {
    int row;
    int col;
};

// Functions
char cellToChar(CellState cell);
void resetBoard(CellState board[8][8]);
void displayBoard(CellState board[8][8]);
void displayMoves(const std::vector<Move>& moves = {});
bool legalMove(Move move, CellState board[8][8], CellState currentPlayer);
std::vector<Move> findAllLegalMoves(CellState board[8][8], CellState currentPlayer);
void makeMove(Move move, CellState board[8][8], CellState currentPlayer);
std::pair<int,int> countDisk(CellState board[8][8]);



#endif //GAMEBOARD_H
