
#ifndef AIENGINE_H
#define AIENGINE_H

#include "../../game_logic/History.h"
#include "../../game_logic/GameEngine.h"

class AIEngine {
private:
    GameEngine game;
    vector<float> state = game.BoardToState();

public:
    AIEngine();
};



#endif //AIENGINE_H
