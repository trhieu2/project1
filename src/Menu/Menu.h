#ifndef MENU_H
#define MENU_H
#include "SDL.h"
#include "SDL_ttf.h"
#include "TextManager.h"
#include "GameFunction.h"

static SDL_Surface* g_img_menu = NULL;


class Menu
{
    public:
        Menu();
        ~Menu();
        int ShowMenu(SDL_Surface* des, TTF_Font* font);
        bool CheckFocusWithRect(const int& x,const int& y,const SDL_Rect& rect);
    private:
};

#endif // MENU_H
