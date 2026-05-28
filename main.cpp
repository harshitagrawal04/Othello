#include <iostream>
#include "game_logic/GameEngine.h"
#include "game_logic/Move.h"
#include "ai_bot/AI.h"
#include "ai_bot/basic/Greedy.h"
#include "ai_bot/advance/HeuristicMinimaxAlphabeta.h"
#include "ai_bot/intermediate/MinimaxAlphabeta.h"
#include "Display/DisplayEngine.h"
using namespace std;


int main() {
    DisplayEngine display;
    display.run();
    return 0;
}
