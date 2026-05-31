#ifndef GETOPPONENT_H
#define GETOPPONENT_H


#include "data_types/DataTypes.h"
#include "ai_bot/AI.h"

inline CellState GetOpponent(CellState player) {
    return player == CellState::Black ?
           CellState::White : CellState::Black;
}

inline pair<CellState, CellState> GetCellState() {
    CellState AI = GetRandomAIState();

    return make_pair(GetOpponent(AI), AI);

}

#endif //GETOPPONENT_H
