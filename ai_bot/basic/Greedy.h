#ifndef GREEDY_H
#define GREEDY_H

#include "game_logic/GameEngine.h"
#include "data_types/DataTypes.h"
#include "ai_bot/AI.h"

class Greedy: public AI{
public:
    // =================== BOT ACTION EXECUTION ===================
    Move SelectMove(GameEngine& engine, CellState botColor) override;
};



#endif //GREEDY_H
