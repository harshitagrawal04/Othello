#ifndef MINIMAXALPHABETABASE_H
#define MINIMAXALPHABETABASE_H

#include "../../game_logic/GameEngine.h"
#include "../../game_logic/Move.h"
#include "../AI.h"



class MinimaxAlphabetaBase {
    int DEPTH = 4;
public:
    double evaluation(GameEngine& engine, CellState BotColor);
    double Alphabeta(GameEngine& engine, int depth, double alpha, double beta, CellState BotColor);
    void SetDepth(int depth) {
        DEPTH = depth;
    }
};



#endif //MINIMAXALPHABETABASE_H
