#ifndef MINIMAXHEURISTICEVAL_H
#define MINIMAXHEURISTICEVAL_H

#include <iostream>
#include "../../game_logic/GameEngine.h"
#include "../../game_logic/Move.h"
#include "../AI.h"

class MinimaxHeuristicEval: public AI {
public:
    Move SelectMove(GameEngine& engine) override;
};



#endif //MINIMAXHEURISTICEVAL_H
