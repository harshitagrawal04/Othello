#ifndef AI_H
#define AI_H

#include "../game_logic/GameEngine.h"

class AI {
public:
    virtual ~AI() = default;
    virtual Move SelectMove(GameEngine& engine) = 0;
};

unique_ptr<AI> createAIAgent(const string& type);

#endif //AI_H
