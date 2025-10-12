#include <iostream>
#include <memory>
#include "AI.h"
#include "../ai_bot/basic/Greedy.h"
#include "../ai_bot/advance/ActorCritic.h"
#include "../ai_bot/intermediate/tabular.h"

// helper function to get the agent to play against
unique_ptr<AI> createAIAgent(const string& type) {
    if (type == "basic") {
        return make_unique<Greedy>();
    }
    if (type == "intermediate") {
        return make_unique<Tabular>();
    }
    if (type == "advance") {
        return make_unique<ActorCritic>();
    }
    return nullptr; // Return a null to play against friend
}