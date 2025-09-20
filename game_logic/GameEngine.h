#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include "Move.h"
#include "History.h"

using namespace std;


class GameEngine {
private:
    // =================== VARIABLES ===================
    CellState board[BOARD_SIZE][BOARD_SIZE] {}; // game board
    vector<History> histories;                   // move history
    unordered_map<Move, vector<Move>> move_map;  // legal moves map
    CellState currentPlayer;                     // current player


    // =================== HELPER FUNCTIONS ===================
    bool InBoard(int row, int col);
    vector<Move> GetFlips(Move move);
    vector<Move> GetFlipsMap(Move current_move);
    CellState GetOpponent();

    // board initialization
    void ResetBoard();

    // display helpers
    void DisplayCell(CellState cell); // LATER MOVE IT TO DISPLAY
    char IntToChar(int Int); // LATER MOVE IT TO DISPLAY
    vector<Move> GetKeys(); // LATER MOVE IT TO DISPLAY


public:

    // =================== UTILITY FUNCTIONS ===================
    int CharToInt(char ch);

    // =================== CONSTRUCTOR ===================
    GameEngine();

    // =================== MAIN FUNCTIONS ===================
    bool IsValidMove(Move move);
    void MakeMove(int row, int col);
    void UndoMove();
    pair<int,int> CountDisk();
    unordered_map<Move, vector<Move>> GetLegalMoves();
    bool GameEnd();

    // =================== DISPLAY FUNCTIONS ===================
    void DisplayBoard(); // LATER MOVE IT TO DISPLAY
    void DisplayMoves(); // LATER MOVE IT TO DISPLAY
    void DisplayHistory(); // LATER MOVE IT TO DISPLAY
    void DisplayPlayer(); // LATER MOVE IT TO DISPLAY
};


#endif //GAME_LOGIC_H
