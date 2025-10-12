#include <iostream>
#include "game_logic/GameEngine.h"
#include "game_logic/Move.h"
#include "ai_bot/AI.h"
#include "ai_bot/basic/Greedy.h"
#include "ai_bot/advance/ActorCritic.h"
#include "ai_bot/intermediate/tabular.h"
#include "Display/DisplayEngine.h"
using namespace std;

/* --------------------------------------------------------------------------
      ADD LOGGING SYSTEM AND UNIT TEST
   -------------------------------------------------------------------------- */


int main() {
    DisplayEngine display;
    display.run();
    return 0;
}
