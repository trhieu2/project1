#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "BaseObject.h"
#include "GameFunction.h"

static SDL_Surface* g_img_menu = NULL;


class GameMenu
{
    public:
        GameMenu();
        ~GameMenu();
        int ShowMenu(SDL_Surface* des, TTF_Font* font);
    private:
};

#endif // GAMEMENU_H
