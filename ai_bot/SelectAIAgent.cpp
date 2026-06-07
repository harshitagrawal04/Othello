#include <iostream>
#include <memory>
#include "ai_bot/AI.h"
#include "ai_bot/basic/Greedy.h"
#include "ai_bot/advance/HeuristicMinimaxAlphabeta.h"
#include "ai_bot/intermediate/MinimaxAlphabeta.h"
#include <random>

// helper function to get the agent to play against
std::unique_ptr<AI> CreateAIAgent(const std::string& type) {
    if (type == "basic") {
        return std::make_unique<Greedy>();
    }
    if (type == "intermediate") {
        return std::make_unique<MinimaxAlphabeta>();
    }
    if (type == "advance") {
        return std::make_unique<HeuristicMinimaxAlphabeta>();
    }
    return nullptr; // Return a null to play against friend
}

CellState GetRandomAIState() {
    // 'static' ensures the engine is only seeded once, not every time the function is called
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, 1); // Generates 0 or 1 with equal probability

    if (distrib(gen) == 0)
        return CellState::Black;
    return CellState::White;
}
