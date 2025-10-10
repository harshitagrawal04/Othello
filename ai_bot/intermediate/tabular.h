
#ifndef TABULAR_H
#define TABULAR_H

#include "../AI.h"

class Tabular : public AI{
public:
    Move SelectMove(GameEngine& engine) override;
};



#endif //TABULAR_H
