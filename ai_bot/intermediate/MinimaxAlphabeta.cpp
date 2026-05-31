#include "MinimaxAlphabeta.h"
#include <iostream>

/* ---------------------------------------------------------------------------------------
                                        CONSTRUCTURE
   ---------------------------------------------------------------------------------------  */

MinimaxAlphabeta::MinimaxAlphabeta() {
    SetDepth(depth);
}

/* ---------------------------------------------------------------------------------------
                                    BOT ACTION EXECUTION
   ---------------------------------------------------------------------------------------  */

// find a move which convert the maximum number of disk
Move MinimaxAlphabeta::SelectMove(GameEngine& engine, CellState botColor) {
    Move bestMove = DefaultMove(engine);
    double bestValue = -1.0 * INFINITY;

    int depth = 0;
    double alpha = -1.0 * INFINITY;
    double beta = INFINITY;

    for (const auto& move : GetOrderedMoves(engine)) {
        engine.MakeMove(move.row, move.col);
        double value = Alphabeta(engine, depth+1, alpha, beta, botColor);
        engine.UndoMove();

        if (value > bestValue) {
            bestValue = value;
            bestMove = move;
            alpha = std::max(alpha, value); // ← add this
        }
    }

    return bestMove;
}