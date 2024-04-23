#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameFunction.h"


class GameState
{
    public:
        virtual void enter()=0;
        virtual void update()=0;
        virtual void exit()=0;
        GameState();
        ~GameState();

    protected:

    private:
};

class Menu : public GameState{
public:
    void enter();
    void update();
    void exit();

};

#endif // GAMESTATE_H
