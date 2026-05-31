
#ifndef AIENGINE_H
#define AIENGINE_H

#include "data_types/DataTypes.h"
#include "game_logic/GameEngine.h"
#include "ai_bot/AI.h"
#include "ai_bot/MinimaxAlphabetaBase/MinimaxAlphabetaBase.h"


class HeuristicMinimaxAlphabeta: public MinimaxAlphabetaBase, public AI {
    // =================== SEARCH CONFIGURATION ===================
    int depth = 7;

    // =================== ADVANCED HEURISTIC EVALUATION ===================
    double CornerCapture(GameEngine& engine, CellState botColor);
    double Stability(GameEngine& engine, CellState botColor);
    double FrontierDiscs(GameEngine& engine, CellState botColor);
    double Evaluation(GameEngine& engine, CellState botColor) override;
public:
    // =================== CONSTRUCTOR ===================
    HeuristicMinimaxAlphabeta();

    // =================== BOT ACTION EXECUTION ===================
    Move SelectMove(GameEngine& engine, CellState botColor) override;
};



#endif //AIENGINE_H
