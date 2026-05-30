
#ifndef AIENGINE_H
#define AIENGINE_H

#include "../../game_logic/History.h"
#include "../../game_logic/GameEngine.h"
#include "../AI.h"
#include "../MinimaxAlphabetaBase/MinimaxAlphabetaBase.h"


class HeuristicMinimaxAlphabeta: public MinimaxAlphabetaBase, public AI {
    int DEPTH = 7;
    double CornerCapture(GameEngine& engine, CellState BotColor);
    double Stability(GameEngine& engine, CellState BotColor);
    double FrontierDiscs(GameEngine& engine, CellState BotColor);
    double Evaluation(GameEngine& engine, CellState BotColor) override;
public:
    HeuristicMinimaxAlphabeta();
    Move SelectMove(GameEngine& engine, CellState BotColor) override;
};



#endif //AIENGINE_H
