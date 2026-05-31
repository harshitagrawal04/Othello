#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <unordered_map>
#include <vector>
#include <tuple>
#include "data_types/DataTypes.h"


class GameEngine {
    // =================== VARIABLES ===================
    CellState board[BOARD_SIZE][BOARD_SIZE] {};  // game board
    std::vector<History> histories;                   // move history
    std::unordered_map<Move, std::vector<Move>> moveMap;  // legal moves map
    CellState currentPlayer;                     // current player


    // =================== HELPER FUNCTIONS ===================
    bool InBoard(int row, int col);
    std::vector<Move> GetFlips(Move move);
    std::vector<Move> GetFlips(Move move, CellState opponent);
    std::vector<Move> GetFlipsMap(Move currentMove);
    void ResetBoard();


public:

    // =================== CONSTRUCTOR ===================
    GameEngine();

    // =================== MAIN FUNCTIONS ===================
    void Reset();
    bool IsValidMove(Move move);
    void MakeMove(int row, int col);
    void UndoMove();
    std::pair<int,int> CountDisk();
    std::unordered_map<Move, std::vector<Move>> GetLegalMoves();
    bool GameEnd();
    std::vector<Move> GetKeys();

    // =================== FUNCTION FOR AI =====================
    void UndoAI();
    std::pair<int,int> FrontierCount();
    std::pair<int, int> StableCount();
    std::pair<int,int> CountLegalMove();

    // =================== GETTER FUNCTIONS ===================
    const CellState (&GetBoard() const)[BOARD_SIZE][BOARD_SIZE] {
        return board;
    }
    const std::vector<History>& GetHistories() const {
        return histories;
    }
    const std::unordered_map<Move, std::vector<Move>>& GetLegalMovesMap() const {
        return moveMap;
    }
    CellState GetCurrentPlayer() const {
        return currentPlayer;
    }
};


#endif //GAME_LOGIC_H
