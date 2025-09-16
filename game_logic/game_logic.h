#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <iostream>
#include <map>

using namespace std;


enum class CellState {
    Empty,
    Black,
    White
};

struct Move {
    int row;
    int col;

    // overloading '<' operator for comparison for map
    bool operator<(const Move& other) const {
        if (row == other.row) {
            return col < other.col;
        }
        return row < other.row;
    }
};

struct History {
    CellState player;
    Move move;
    vector<Move> flipped;
};


constexpr int BOARD_SIZE = 8;

// helper function
bool InBoard(int row, int col);
void DisplayCell(CellState cell);
CellState GetOpponent(CellState current_player);
vector<Move> GetKeys(const map<Move, vector<Move>>& moves);
vector<Move> GetFlipsMap(const map<Move, vector<Move>>& moves, Move current_move);


// main function
void ResetBoard(CellState board[BOARD_SIZE][BOARD_SIZE]);
vector<Move> GetFlips(CellState board[BOARD_SIZE][BOARD_SIZE], Move move, CellState current_player);
map<Move, vector<Move>> GetLegalMoves(CellState board[BOARD_SIZE][BOARD_SIZE], CellState current_player);
void MakeMove(CellState board[BOARD_SIZE][BOARD_SIZE], CellState current_player, Move move, const vector<Move>& flips);
pair<int,int> CountDisk(CellState board[BOARD_SIZE][BOARD_SIZE]);
void UndoMove(CellState board[BOARD_SIZE][BOARD_SIZE], vector<History>& histories);


// display function
void DisplayMoves(const map<Move, vector<Move>>& move);
void DisplayBoard(CellState board[BOARD_SIZE][BOARD_SIZE]);
void DisplayHistory(const History& history);




#endif //GAME_LOGIC_H
