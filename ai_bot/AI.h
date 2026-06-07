#ifndef AI_H
#define AI_H

#include "game_logic/GameEngine.h"

class AI {
public:
    virtual ~AI() = default;
    virtual Move SelectMove(GameEngine& engine, CellState botColor) = 0;
};

// helper function to get the agent to play against
std::unique_ptr<AI> CreateAIAgent(const std::string& type);

// helper function to get the Cellstate colour for the ai
CellState GetRandomAIState();

#endif //AI_H
