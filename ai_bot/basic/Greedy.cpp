#include "Greedy.h"
#include "../../game_logic/GameEngine.h"

// find a move which convert the maximum number of disk
Move Greedy::SelectMove(GameEngine& engine) {
    unordered_map<Move, vector<Move>> move_map = engine.GetLegalMoves();
    Move BestMove{};
    int max = 0;

    for (const auto& [move, flips]: move_map) {
        if (flips.size() > max) {
            BestMove = move;
            max = flips.size();
        }
    }
    return BestMove;
}