#include "MinimaxAlphabetaBase.h"


const int CORNER_DIRECTIONS[3][2] = { {0, 1}, {1, 0}, {1, 1} };

double MinimaxAlphabetaBase::evaluation(GameEngine& engine, CellState BotColor) {
    // coin parity
    auto [black_disk, white_disk] = engine.CountDisk();
    double coin_parity = 0;
    if (BotColor == CellState::White)
        coin_parity = 100.0 * (white_disk - black_disk)/(white_disk + black_disk);
    else
        coin_parity = 100.0 * (black_disk - white_disk)/(white_disk + black_disk);

    // mobility
    double my_moves, opp_moves;
    auto [black_moves, white_moves] = engine.CountLegalMove();

    if (BotColor == CellState::Black) {
        my_moves = black_moves;
        opp_moves = white_moves;
    }
    else {
        my_moves = white_moves;
        opp_moves = black_moves;
    }

    double mobility = (my_moves + opp_moves > 0)
                    ? 100.0 * (my_moves - opp_moves) / (my_moves + opp_moves)
                    : 0;

    //Corner proximity penalty
    int corner_penalty_count = 0;
    int corners[4][2] = { {0, 0}, {0, 7}, {7, 0}, {7, 7} };
    const auto& board = engine.GetBoard();
    CellState my_color = BotColor;
    CellState opp_color = BotColor == CellState::Black ?
                          CellState::White : CellState::Black;;

    for (auto& corner : corners) {
        int cr = corner[0];
        int cc = corner[1];

        // Only penalize if the corner is empty!
        if (board[cr][cc] == CellState::Empty) {

            // Check the 3 adjacent squares to this specific corner
            // Top-Left corner (0,0) checks (0,1), (1,0), (1,1)
            // Bottom-Right corner (7,7) checks (6,7), (7,6), (6,6), etc.
            int step_r = (cr == 0) ? 1 : -1;
            int step_c = (cc == 0) ? 1 : -1;

            int neighbors[3][2] = {
                {cr, cc + step_c}, // C-Square
                {cr + step_r, cc}, // C-Square
                {cr + step_r, cc + step_c} // X-Square
            };

            for (auto& adj : neighbors) {
                if (board[adj[0]][adj[1]] == my_color) {
                    corner_penalty_count++;
                }
                // If opponent sits there, it's actually good for us (they might give us the corner)
                else if (board[adj[0]][adj[1]] == opp_color) {
                    corner_penalty_count--;
                }
            }
        }
    }

    return (10.0f * coin_parity) + (5.0f * mobility) - (8.0f * static_cast<float>(corner_penalty_count));
}

double MinimaxAlphabetaBase::Alphabeta(GameEngine& engine, int depth, double alpha, double beta, CellState BotColor) {
    if (depth >= DEPTH || engine.GameEnd()) {
        return evaluation(engine, BotColor);
    }

    if (engine.GetCurrentPlayer() == BotColor) {
        unordered_map<Move, vector<Move>> move_map = engine.GetLegalMoves();
        double max = -1.0 * INFINITY;
        for (const auto& [move, flips]: move_map) {
            engine.MakeMove(move.row, move.col);
            double value = Alphabeta(engine, depth+1, alpha, beta, BotColor);
            engine.UndoMove();
            max = std::max(max, value);
            alpha = std::max(alpha, value);

            if (beta <= alpha)
                break;
        }

        return max;
    }

    unordered_map<Move, vector<Move>> move_map = engine.GetLegalMoves();
    double min = INFINITY;
    for (const auto& [move, flips]: move_map) {
        engine.MakeMove(move.row, move.col);
        double value = Alphabeta(engine, depth+1, alpha, beta, BotColor);
        engine.UndoMove();
        min = std::min(min, value);
        beta = std::min(beta, value);

        if (beta <= alpha)
            break;
    }

    return min;
}