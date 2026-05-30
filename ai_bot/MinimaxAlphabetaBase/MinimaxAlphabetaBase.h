#ifndef MINIMAXALPHABETABASE_H
#define MINIMAXALPHABETABASE_H

#include "../../game_logic/GameEngine.h"
#include "../../game_logic/Move.h"
#include "../AI.h"



class MinimaxAlphabetaBase {
    int DEPTH = 4;
public:
    CellState GetOpponent(CellState player);
    vector<Move> GetOrderedMoves(GameEngine& engine);
    double CoinParity(GameEngine& engine, CellState BotColor);
    double Mobility(GameEngine& engine, CellState BotColor);
    double CornerProximityPenalty(GameEngine& engine, CellState BotColor);
    virtual double Evaluation(GameEngine& engine, CellState BotColor);
    double Alphabeta(GameEngine& engine, int depth, double alpha, double beta, CellState BotColor);
    void SetDepth(int depth) {
        DEPTH = depth;
    }
};



#endif //MINIMAXALPHABETABASE_H
