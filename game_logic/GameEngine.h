#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <iostream>
#include <unordered_map>
#include "Move.h"
#include "History.h"

using namespace std;


class GameEngine {
private:
    // object variable
    CellState board[BOARD_SIZE][BOARD_SIZE] {};
    vector<History> histories;


    // helper function
    bool InBoard(int row, int col);
    void DisplayCell(CellState cell); // LATER MOVE IT TO DISPLAY
    vector<Move> GetKeys(); // LATER MOVE IT TO DISPLAY
    vector<Move> GetFlipsMap(Move current_move);
    char IntToChar(int Int); // LATER MOVE IT TO DISPLAY

    // main function
    void ResetBoard();
    vector<Move> GetFlips(Move move);


public:
    CellState currentPlayer;
    unordered_map<Move, vector<Move>> move_map;

    int CharToInt(char ch); // LATER MOVE IT TO DISPLAY

    // constructor
    GameEngine();

    // main function
    bool IsValidMove(Move move);
    void MakeMove(int row, int col);
    pair<int,int> CountDisk();
    void UndoMove();
    CellState GetOpponent();
    unordered_map<Move, vector<Move>> GetLegalMoves();



    // LATER MOVE IT TO DISPLAY

    // display function
    void DisplayMoves();
    void DisplayBoard();
    void DisplayHistory();
};


#endif //GAME_LOGIC_H
