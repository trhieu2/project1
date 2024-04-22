#include "GameMenu.h"

GameMenu::GameMenu()
{
    //ctor
}

GameMenu::~GameMenu()
{
    //dtor
}

int GameMenu::ShowMenu(SDL_Surface* des, TTF_Font* font)
{
    g_img_menu = Loadimage("start_screen.png");
}
