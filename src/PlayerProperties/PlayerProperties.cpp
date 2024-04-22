#include "PlayerProperties.h"

PlayerProperties::PlayerProperties()
{
    number_ = 0;
}

PlayerProperties::~PlayerProperties()
{
    //dtor
}

void PlayerProperties::AddPos(const int& xPos)
{
    post_list.push_back(xPos);
}

void PlayerProperties::Show(SDL_Renderer* screen)
{
    for(int i = 0; i < post_list.size(); i++)
    {
        m_rect.x = post_list.at(i);
        m_rect.y = 0;
        Render(screen);
    }
}

void PlayerProperties::Init(SDL_Renderer* screen)
{
    LoadImg("assets/player_pw.png", screen);
    number_ = 3;
    if(post_list.size() > 0)
    {
        post_list.clear();
    }

    AddPos(20);
    AddPos(60);
    AddPos(100);
}

void PlayerProperties::Increase()
{
    number_++;
    int last_pos = post_list.back();
    last_pos += 40;
    post_list.push_back(last_pos);
}

void PlayerProperties::Decrease()
{
    number_--;
    post_list.pop_back();
}

