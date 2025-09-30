#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>
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
    vector<Move> GetKeys(); // LATER MOVE IT TO DISPLAY


public:

    // =================== UTILITY FUNCTIONS ===================
    int CharToInt(char ch);
    char IntToChar(int Int); // LATER MOVE IT TO DISPLAY

    // =================== CONSTRUCTOR ===================
    GameEngine();

    // =================== MAIN FUNCTIONS ===================
    void Reset();
    bool IsValidMove(Move move);
    void MakeMove(int row, int col);
    void UndoMove();
    pair<int,int> CountDisk();
    unordered_map<Move, vector<Move>> GetLegalMoves();
    bool GameEnd();

    // =================== FUNCTION FOR AI =====================
    vector<float> BoardToState();
    unordered_map<int, vector<int>> LegalMoveMapToStateMap();
    vector<int> StateMapIndices();
    tuple<vector<float>, float, bool> Step(int actionIndex); // actor-critic
    void UndoAI();


    // =================== DISPLAY FUNCTIONS ===================
    void DisplayBoard(); // LATER MOVE IT TO DISPLAY
    void DisplayMoves(); // LATER MOVE IT TO DISPLAY
    void DisplayHistory(); // LATER MOVE IT TO DISPLAY
    void DisplayPlayer(); // LATER MOVE IT TO DISPLAY

    // =================== GETTER FUNCTIONS ===================
    const CellState (&GetBoard() const)[BOARD_SIZE][BOARD_SIZE] {
        return board;
    }
    const vector<History>& GetHistories() const {
        return histories;
    }
    const unordered_map<Move, vector<Move>>& GetLegalMovesMap() const {
        return move_map;
    }
    CellState GetCurrentPlayer() const {
        return currentPlayer;
    }

    /*
     * GameEngine game;
     * const auto& board = game.GetBoard();
     * const auto& legalMoves = game.GetLegalMovesMap();
     * const auto& history = game.GetHistories();
     * CellState player = game.GetCurrentPlayer();
     */
};


#endif //GAME_LOGIC_H
