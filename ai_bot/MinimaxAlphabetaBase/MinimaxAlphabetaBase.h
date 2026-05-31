#ifndef MINIMAXALPHABETABASE_H
#define MINIMAXALPHABETABASE_H

#include "game_logic/GameEngine.h"
#include "data_types/DataTypes.h"



class MinimaxAlphabetaBase {
    // =================== SEARCH CONFIGURATION ===================
    int DEPTH = 4;
public:
    virtual ~MinimaxAlphabetaBase() = default;

    // =================== SEARCH SETTERS ===================
    void SetDepth(int depth) { DEPTH = depth; }

    // =================== SEARCH MOVE GENERATION ===================
    std::vector<Move> GetOrderedMoves(GameEngine& engine);

    // =================== BASIC HEURISTIC EVALUATION ===================
    double CoinParity(GameEngine& engine, CellState botColor);
    double Mobility(GameEngine& engine, CellState botColor);
    double CornerProximityPenalty(GameEngine& engine, CellState botColor);
    virtual double Evaluation(GameEngine& engine, CellState botColor);

    // ===================== BASE CASE IF NO MOVE FOUND ===================
    Move DefaultMove(GameEngine& engine);

    // =================== CORE SEARCH ALGORITHM ===================
    double Alphabeta(GameEngine& engine, int depth, double alpha, double beta, CellState botColor);
};



#endif //MINIMAXALPHABETABASE_H
