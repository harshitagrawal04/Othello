#include "HeuristicMinimaxAlphabeta.h"



Move HeuristicMinimaxAlphabeta::SelectMove(GameEngine& engine, CellState BotColor) {
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
