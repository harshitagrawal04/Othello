#include "Greedy.h"
#include "game_logic/GameEngine.h"

/* ---------------------------------------------------------------------------------------
                                    BOT ACTION EXECUTION
   ---------------------------------------------------------------------------------------  */

// find a move which convert the maximum number of disk
Move Greedy::SelectMove(GameEngine& engine, CellState botColor) {
    std::unordered_map<Move, std::vector<Move>> moveMap = engine.GetLegalMoves();
    Move bestMove{};
    int max = 0;

    for (const auto& [move, flips]: moveMap) {
        if (flips.size() > max) {
            bestMove = move;
            max = flips.size();
        }
    }
    return bestMove;
}