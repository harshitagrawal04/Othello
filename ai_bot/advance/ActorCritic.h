
#ifndef AIENGINE_H
#define AIENGINE_H

#include "../../game_logic/History.h"
#include "../../game_logic/GameEngine.h"
#include "../AI.h"


class ActorCritic : public AI{
public:
    Move SelectMove(GameEngine& engine) override;
};



#endif //AIENGINE_H
