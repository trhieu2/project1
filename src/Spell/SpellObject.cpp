#include "SpellObject.h"

SpellObject::SpellObject()
{
    x_val = 0;
    y_val = 0;
    is_moving = false;
    spell_type = RINGED;
}

SpellObject::~SpellObject()
{

}

void SpellObject::LoadImgSpell(SDL_Renderer* des)
{
    if(spell_type == RINGED)
    {
        LoadImg("assets/demospell.png", des);
    }
    else if(spell_type == NORING)
    {
        LoadImg("assets/noring.png", des);
    }
}


void SpellObject::HandleMove(const int& x_border, const int& y_border)
{
    if(spell_direction == DIR_RIGHT)
    {
        m_rect.x += x_val;
    if(m_rect.x > x_border)
    {
        is_moving = false;
    }
    }
    else if(spell_direction == DIR_LEFT)
    {
        m_rect.x -= x_val;
        if(m_rect.x < 0)
        {
            is_moving = false;
        }
    }
    else if(spell_direction == DIR_UP)
    {
        m_rect.y -= y_val;
        if(m_rect.y < 0)
        {
            is_moving = false;
        }
    }
    else if(spell_direction == DIR_UP_LEFT)
    {
        m_rect.x -= x_val;
        if(m_rect.x < 0)
        {
            is_moving = false;
        }

        m_rect.y -= y_val;
        if(m_rect.y < 0)
        {
            is_moving = false;
        }
    }
    else if(spell_direction == DIR_UP_RIGHT)
    {
        m_rect.x += x_val;
        if(m_rect.x > x_border)
        {
            is_moving = false;
        }

        m_rect.y -= y_val;
        if(m_rect.y < 0)
        {
            is_moving = false;
        }
    }
}

