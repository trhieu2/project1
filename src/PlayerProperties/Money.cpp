#include "Money.h"

Money::Money()
{
    x_pos = 0;
    y_pos = 0;
}

Money::~Money()
{
    //dtor
}

void Money::Init(SDL_Renderer* screen)
{
    bool ret = LoadImg("assets/money_img.png", screen);
}

void Money::Show(SDL_Renderer* screen)
{
    m_rect.x = x_pos;
    m_rect.y = y_pos;
    Render(screen);
}

