#ifndef GREEDY_H
#define GREEDY_H

#include <iostream>
#include "../../game_logic/GameEngine.h"
#include "../../game_logic/Move.h"
#include "../AI.h"

class Greedy: public AI{
public:
    Move SelectMove(GameEngine& engine) override;
    // void PlayMove(GameEngine& engine);
};



#endif //GREEDY_H
