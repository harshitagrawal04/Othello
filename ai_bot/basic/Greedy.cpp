#include "Greedy.h"
#include "../../game_logic/GameEngine.h"


Move Greedy::SelectMove(GameEngine& engine) {
    unordered_map<Move, vector<Move>> move_map = engine.GetLegalMoves();
    Move BestMove{};
    int max = 0;

    for (const auto& [move, flips]: move_map) {
        if (flips.size() > max) {
            BestMove = move;
            max = flips.size();
        }
    }

    return BestMove;
}

// void Greedy::PlayMove(GameEngine& engine) {
//     Move move = SelectMove(engine);
//     engine.Step(move);  // or whatever applies the move in your engine
// }