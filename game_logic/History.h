#ifndef HISTORY_H
#define HISTORY_H

#include "Move.h"
#include <vector>

using namespace std;



constexpr int BOARD_SIZE = 8;

enum class CellState {
    Empty,
    Black,
    White
};


struct History {
    CellState player;
    Move move;
    vector<Move> flipped;
    CellState board[BOARD_SIZE][BOARD_SIZE];
};




#endif //HISTORY_H
