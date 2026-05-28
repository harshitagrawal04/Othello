#include "MinimaxAlphabeta.h"
#include <iostream>
#include <ranges>


void MinimaxAlphabeta::SetBase() {
    BASE.SetDepth(DEPTH);
}


// find a move which convert the maximum number of disk
Move MinimaxAlphabeta::SelectMove(GameEngine& engine, CellState BotColor) {
    unordered_map<Move, vector<Move>> move_map = engine.GetLegalMoves();
    Move bestMove{};
    double bestValue = -1.0 * INFINITY;

    int depth = 0;
    double alpha = -1.0 * INFINITY;
    double beta = INFINITY;

    for (const auto &move: move_map | views::keys) {
        engine.MakeMove(move.row, move.col);
        double value = BASE.Alphabeta(engine, depth+1, alpha, beta, BotColor);
        engine.UndoMove();

        if (value > bestValue) {
            bestValue = value;
            bestMove = move;
            alpha = std::max(alpha, value); // ← add this
        }
    }

    return bestMove;
}