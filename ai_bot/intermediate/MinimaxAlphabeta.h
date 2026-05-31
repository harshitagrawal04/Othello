#ifndef MINIMAXHEURISTICEVAL_H
#define MINIMAXHEURISTICEVAL_H

#include "game_logic/GameEngine.h"
#include "data_types/DataTypes.h"
#include "ai_bot/AI.h"
#include "ai_bot/MinimaxAlphabetaBase/MinimaxAlphabetaBase.h"


class MinimaxAlphabeta: public MinimaxAlphabetaBase, public AI {
    // =================== SEARCH CONFIGURATION ===================
    int depth = 4;
public:
    // =================== CONSTRUCTOR ===================
    MinimaxAlphabeta();

    // =================== BOT ACTION EXECUTION ===================
    Move SelectMove(GameEngine& engine, CellState BotColor) override;
};



#endif //MINIMAXHEURISTICEVAL_H
