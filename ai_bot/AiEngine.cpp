#include "AiEngine.h"



//default construct
AIEngine::AIEngine() {
    // initialize the state vector
    state = vector(BOARD_SIZE * BOARD_SIZE, 0.0f);
}
