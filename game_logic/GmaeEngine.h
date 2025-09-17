#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <iostream>
#include <unordered_map>
#include "Move.h"
#include "History.h"

using namespace std;



// helper function
bool InBoard(int row, int col);
void DisplayCell(CellState cell);
CellState GetOpponent(CellState current_player);
vector<Move> GetKeys(const unordered_map<Move, vector<Move>>& moves);
vector<Move> GetFlipsMap(const unordered_map<Move, vector<Move>>& moves, Move current_move);
char IntToChar(int Int);
int CharToInt(char ch);


// main function
void ResetBoard(CellState board[BOARD_SIZE][BOARD_SIZE]);
vector<Move> GetFlips(CellState board[BOARD_SIZE][BOARD_SIZE], Move move, CellState current_player);
unordered_map<Move, vector<Move>> GetLegalMoves(CellState board[BOARD_SIZE][BOARD_SIZE], CellState current_player);
void MakeMove(CellState board[BOARD_SIZE][BOARD_SIZE], CellState current_player, Move move, const vector<Move>& flips);
pair<int,int> CountDisk(CellState board[BOARD_SIZE][BOARD_SIZE]);
void UndoMove(CellState board[BOARD_SIZE][BOARD_SIZE], vector<History>& histories);


// display function
void DisplayMoves(const unordered_map<Move, vector<Move>>& move);
void DisplayBoard(CellState board[BOARD_SIZE][BOARD_SIZE]);
void DisplayHistory(const History& history);




#endif //GAME_LOGIC_H
