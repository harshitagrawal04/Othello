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
    CellState board[BOARD_SIZE][BOARD_SIZE];
    CellState currentPlayer;
    vector<History> histories;


    // helper function
    bool InBoard(int row, int col);
    void DisplayCell(CellState cell); // LATER MOVE IT TO DISPLAY
    CellState GetOpponent();
    CellState GetOpponent(CellState player);
    vector<Move> GetKeys(const unordered_map<Move, vector<Move>>& moves); // LATER MOVE IT TO DISPLAY
    vector<Move> GetFlipsMap(const unordered_map<Move, vector<Move>>& moves, Move current_move);
    char IntToChar(int Int); // LATER MOVE IT TO DISPLAY
    int CharToInt(char ch); // LATER MOVE IT TO DISPLAY

    // main function
    void ResetBoard();
    vector<Move> GetFlips(Move move);
    unordered_map<Move, vector<Move>> GetLegalMoves();


public:
    // constructor
    GameEngine();

    // main function
    void MakeMove(Move move, const vector<Move>& flips);
    pair<int,int> CountDisk();
    void UndoMove();


    // LATER MOVE IT TO DISPLAY

    // display function
    void DisplayMoves(const unordered_map<Move, vector<Move>>& move);
    void DisplayBoard();
    void DisplayHistory();
};


#endif //GAME_LOGIC_H
