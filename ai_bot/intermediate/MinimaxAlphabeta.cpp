#include "MinimaxAlphabeta.h"
#include <iostream>


MinimaxAlphabeta::MinimaxAlphabeta() {
    SetDepth(DEPTH);
}


// find a move which convert the maximum number of disk
Move MinimaxAlphabeta::SelectMove(GameEngine& engine, CellState BotColor) {
    Move bestMove{};
    double bestValue = -1.0 * INFINITY;

    int depth = 0;
    double alpha = -1.0 * INFINITY;
    double beta = INFINITY;

    for (const auto& move : GetOrderedMoves(engine)) {
        engine.MakeMove(move.row, move.col);
        double value = Alphabeta(engine, depth+1, alpha, beta, BotColor);
        engine.UndoMove();

        if (value > bestValue) {
            bestValue = value;
            bestMove = move;
            alpha = std::max(alpha, value); // ← add this
        }
    }

    return bestMove;
}