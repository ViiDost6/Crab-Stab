#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState
{
    public:
        GameState();

        int lvl;
        int stylus_x;
        int stylus_y;
        int marcelo_x;
        int marcelo_y;
        int ant_y;
        int flex;

        void Reset();
};
#endif