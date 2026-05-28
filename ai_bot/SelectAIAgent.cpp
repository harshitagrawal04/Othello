#include <iostream>
#include <memory>
#include "AI.h"
#include "../ai_bot/basic/Greedy.h"
#include "../ai_bot/advance/HeuristicMinimaxAlphabeta.h"
#include "../ai_bot/intermediate/MinimaxAlphabeta.h"

// helper function to get the agent to play against
unique_ptr<AI> createAIAgent(const string& type) {
    if (type == "basic") {
        return make_unique<Greedy>();
    }
    if (type == "intermediate") {
        return make_unique<MinimaxAlphabeta>();
    }
    if (type == "advance") {
        return make_unique<HeuristicMinimaxAlphabeta>();
    }
    return nullptr; // Return a null to play against friend
}