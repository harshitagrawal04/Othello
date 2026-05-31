#include "MinimaxAlphabetaBase.h"
#include "data_types/CommonFunctions.h"

/* ---------------------------------------------------------------------------------------
                                        VARIABLE
   ---------------------------------------------------------------------------------------  */

int POSITION_WEIGHTS[8][8] = {
    { 100, -20,  10,  10,  10,  10, -20, 100 },
    { -20, -50,   0,   0,   0,   0, -50, -20 },
    {  10,   0,   5,   5,   5,   5,   0,  10 },
    {  10,   0,   5,   5,   5,   5,   0,  10 },
    {  10,   0,   5,   5,   5,   5,   0,  10 },
    {  10,   0,   5,   5,   5,   5,   0,  10 },
    { -20, -50,   0,   0,   0,   0, -50, -20 },
    { 100, -20,  10,  10,  10,  10, -20, 100 }
};
const int CORNER_DIRECTIONS[3][2] = { {0, 1}, {1, 0}, {1, 1} };
const int CORNERS[4][2] = { {0, 0}, {0, 7}, {7, 0}, {7, 7} };

/* ---------------------------------------------------------------------------------------
                                  SEARCH MOVE GENERATION
   ---------------------------------------------------------------------------------------  */

std::vector<Move> MinimaxAlphabetaBase::GetOrderedMoves(GameEngine& engine) {
    std::vector<Move> moves = engine.GetKeys();
    std::ranges::sort(moves, [&](const Move& a, const Move& b) {
        return POSITION_WEIGHTS[a.row][a.col] > POSITION_WEIGHTS[b.row][b.col];
    });
    return moves;
}

/* ---------------------------------------------------------------------------------------
                                   BASIC HEURISTIC EVALUATION
   ---------------------------------------------------------------------------------------  */

double MinimaxAlphabetaBase::CoinParity(GameEngine& engine, CellState botColor) {
    auto [black_disk, white_disk] = engine.CountDisk();
    if (botColor == CellState::White)
        return  100.0 * (white_disk - black_disk)/(white_disk + black_disk);
    return 100.0 * (black_disk - white_disk)/(white_disk + black_disk);
}

double MinimaxAlphabetaBase::Mobility(GameEngine& engine, CellState botColor) {
    double bot_moves, opp_moves;
    auto [black_moves, white_moves] = engine.CountLegalMove();

    if (botColor == CellState::Black) {
        bot_moves = black_moves;
        opp_moves = white_moves;
    }
    else {
        bot_moves = white_moves;
        opp_moves = black_moves;
    }

    return  bot_moves + opp_moves > 0
            ? 100.0 * (bot_moves - opp_moves) / (bot_moves + opp_moves)
            : 0;
}

double MinimaxAlphabetaBase::CornerProximityPenalty(GameEngine& engine, CellState botColor) {
    int corner_penalty_count = 0;

    const auto& board = engine.GetBoard();
    CellState my_color = botColor;
    CellState opp_color = GetOpponent(botColor);

    for (auto& corner : CORNERS) {
        int cr = corner[0];
        int cc = corner[1];

        // Only penalize if the corner is empty!
        if (board[cr][cc] == CellState::Empty) {

            int step_r = cr == 0 ? 1 : -1;
            int step_c = cc == 0 ? 1 : -1;

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

    return corner_penalty_count;
}

double MinimaxAlphabetaBase::Evaluation(GameEngine& engine, CellState botColor) {

    return   10.0f * CoinParity(engine, botColor)
           + 5.0f * Mobility(engine, botColor)
           - 8.0f * CornerProximityPenalty(engine, botColor);
}

/* ---------------------------------------------------------------------------------------
                                  BASE CASE IF NO MOVE FOUND
   ---------------------------------------------------------------------------------------  */
Move MinimaxAlphabetaBase::DefaultMove(GameEngine &engine) {
    auto orderedMoves = GetOrderedMoves(engine);

    if (orderedMoves.empty()) {
        return Move{};
    }
    return orderedMoves[0];
}


/* ---------------------------------------------------------------------------------------
                                    CORE SEARCH ALGORITHM
   ---------------------------------------------------------------------------------------  */

double MinimaxAlphabetaBase::Alphabeta(GameEngine& engine, int depth, double alpha, double beta, CellState botColor) {
    if (depth >= DEPTH || engine.GameEnd()) {
        return Evaluation(engine, botColor);
    }

    // for bot maximize the value
    if (engine.GetCurrentPlayer() == botColor) {
        double max = -1.0 * INFINITY;

        for (const auto& move : GetOrderedMoves(engine)) {
            engine.MakeMove(move.row, move.col);
            double value = Alphabeta(engine, depth+1, alpha, beta, botColor);
            engine.UndoMove();
            max = std::max(max, value);
            alpha = std::max(alpha, value);

            if (beta <= alpha)
                break;
        }

        return max;
    }

    // for player minimize the value
    double min = INFINITY;

    for (const auto& move : GetOrderedMoves(engine)) {
        engine.MakeMove(move.row, move.col);
        double value = Alphabeta(engine, depth+1, alpha, beta, botColor);
        engine.UndoMove();
        min = std::min(min, value);
        beta = std::min(beta, value);

        if (beta <= alpha)
            break;
    }

    return min;
}
