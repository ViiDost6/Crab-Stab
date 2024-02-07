#ifndef FORWARDMODEL_H
#define FORWARDMODEL_H

#include "GameState.h"
#include "Action.h"

class ForwardModel
{
    public:
        ForwardModel();
        void ChangeLvl(GameState& gs, Action& action);
        void MarceloMoves(GameState& gs, Action& action);
        void Flexing(GameState& gs, Action& action);
};
#endif