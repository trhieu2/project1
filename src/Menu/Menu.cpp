#include "Menu.h"

Menu::Menu()
{
    //ctor
}

Menu::~Menu()
{
    //dtor
}

int Menu::ShowMenu(SDL_Surface* des, TTF_Font* font)
{
    g_img_menu = IMG_Load("assets/start_screen.png");
    if(g_img_menu == NULL)
        return 1;

    const int kMenuItemNum = 2;
    SDL_Rect pos_arr[kMenuItemNum];
    pos_arr[0].x = 200;
    pos_arr[0].y = 400;

    pos_arr[1].x = 200;
    pos_arr[1].y = 200;

    TextManager text_menu[kMenuItemNum];
    text_menu[0].SetText("PLAY");
    text_menu[0].SetColor(TextManager::BLACK_TEXT);
    text_menu[0].SetRect(pos_arr[0].x, pos_arr[0].y);

    text_menu[1].SetText("EXIT");
    text_menu[1].SetColor(TextManager::BLACK_TEXT);
    text_menu[1].SetRect(pos_arr[1].x, pos_arr[1].y);

    bool selected[kMenuItemNum] = {0,0};

    int xm = 0;
    int ym = 0;

    SDL_Event m_event;
    while(true)
    {
        SDLCommonFunc::ApplySurface(g_img_menu, des, 0, 0);
        for(int i = 0; i < kMenuItemNum; i++)
        {
            text_menu[i].LoadFromRenderText(font, des);
        }
        while(SDL_PollEvent(&m_event))
        {
            switch(m_event.type)
            {
            case SDL_QUIT:
                return 1;
            case SDL_MOUSEMOTION:
                {
                    xm = m_event.motion.x;
                    ym = m_event.motion.y;
                    for(int i = 0; i < kMenuItemNum; i++)
                    {
                        if(CheckFocusWithRect(xm, ym, text_menu[i].GetRect()))
                        {
                            if(selected[i] == false)
                            {
                                selected[i] = 1;
                                text_menu[i].SetColor(TextManager::RED_TEXT);
                            }
                        }
                        else{
                            if(selected[i] == true)
                            {
                                selected[i] = 0;
                                text_menu[i].SetColor(TextManager::BLACK_TEXT);
                            }
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }
}

bool Menu::CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect)
{
    if(x >= rect.x && x <= rect.x + rect.w &&
       y >= rect.y && y <= rect.y + rect.h)
    {
        return true;
    }
    return false;
}


