#include "HeuristicMinimaxAlphabeta.h"


const int CORNERS[4][2] = { {0, 0}, {0, 7}, {7, 0}, {7, 7} };

double HeuristicMinimaxAlphabeta::CornerCapture(GameEngine& engine, CellState BotColor) {
    const auto& board = engine.GetBoard();
    CellState OppColor = GetOpponent(BotColor);
    double penalty = 25.0f;

    double corner_capture = 0;

    for (auto& corner : CORNERS) {
        int cr = corner[0];
        int cc = corner[1];

        if (board[cr][cc] == BotColor) {
            corner_capture += penalty;
        }
        else if (board[cr][cc] == OppColor) {
            corner_capture -= penalty;
        }
    }

    return corner_capture;
}

double HeuristicMinimaxAlphabeta::Stability(GameEngine& engine, CellState BotColor) {
    auto [stable_black, stable_white] = engine.StableCount();

    if (BotColor == CellState::Black)
        return 100.0 * (stable_black - stable_white)/(stable_white + stable_black);
    return 100.0 * (stable_white - stable_black)/(stable_white + stable_black);
}

double HeuristicMinimaxAlphabeta::FrontierDiscs(GameEngine& engine, CellState BotColor) {
    auto [frontier_black, frontier_white] = engine.FrontierCount();

    if (BotColor == CellState::Black)
        return 100.0 * (frontier_black - frontier_white)/(frontier_white + frontier_black);
    return 100.0 * (frontier_white - frontier_black)/(frontier_white + frontier_black);
}

double HeuristicMinimaxAlphabeta::Evaluation(GameEngine& engine, CellState BotColor) {
    return   30.0f * CornerCapture(engine, BotColor)
           + 15.0f * Stability(engine, BotColor)
           + 5.0f  * Mobility(engine, BotColor)
           - 10.0f * FrontierDiscs(engine, BotColor)
           - 12.0f * CornerProximityPenalty(engine, BotColor);
}

HeuristicMinimaxAlphabeta::HeuristicMinimaxAlphabeta() {
    SetDepth(DEPTH);
}

Move HeuristicMinimaxAlphabeta::SelectMove(GameEngine& engine, CellState BotColor) {
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
