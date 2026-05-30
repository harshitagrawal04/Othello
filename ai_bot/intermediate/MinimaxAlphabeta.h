#ifndef MINIMAXHEURISTICEVAL_H
#define MINIMAXHEURISTICEVAL_H

#include "../../game_logic/GameEngine.h"
#include "../../game_logic/Move.h"
#include "../AI.h"
#include "../MinimaxAlphabetaBase/MinimaxAlphabetaBase.h"



class MinimaxAlphabeta: public MinimaxAlphabetaBase, public AI {
    int DEPTH = 4;
public:
    MinimaxAlphabeta();
    Move SelectMove(GameEngine& engine, CellState BotColor) override;
};



#endif //MINIMAXHEURISTICEVAL_H
