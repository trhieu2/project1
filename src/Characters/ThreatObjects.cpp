#include "ThreatObjects.h"

ThreatObjects::ThreatObjects()
{
    width_frame = 0;
    height_frame = 0;
    x_val = 0.0;
    y_val = 0.0;
    x_pos = 0.0;
    y_pos = 0.0;
    isgrounded = 0;
    come_back_time = 0;
    frame = 0;

    animation_a = 0;
    animation_b = 0;
    input_type.left = 0;
    type_move = STATIC_THREAT;
}

ThreatObjects::~ThreatObjects()
{

}

SDL_Rect ThreatObjects::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = m_rect.x;
    rect.y = m_rect.y;
    rect.w = width_frame;
    rect.h = height_frame;
    return rect;
}

void ThreatObjects::set_clips()
{
    if(width_frame > 0 && height_frame > 0)
    {
        for(int k = 0; k < 3; k++){
        frame_clip_[k].x = width_frame*k;
        frame_clip_[k].y = 0;
        frame_clip_[k].w = width_frame;
        frame_clip_[k].h = height_frame;
        }
    }
}

bool ThreatObjects::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if(ret)
    {
        width_frame = m_rect.w/THREAT_FRAME_NUM;
        height_frame = m_rect.h;
    }
    return ret;
}

void ThreatObjects::Show(SDL_Renderer* des)
{
    if(come_back_time == 0)
    {
        m_rect.x = x_pos - map_x;
        m_rect.y = y_pos - map_y;
        frame++;
        if(frame >= 3)
        {
            frame = 0;
        }

        SDL_Rect* currentclip = &frame_clip_[frame];
        SDL_Rect rectQuad = {m_rect.x, m_rect.y, width_frame, height_frame};
        SDL_RenderCopy(des, m_object, currentclip, &rectQuad);
    }
}

void ThreatObjects::DoPlayer(Map& map_data)
{
    if(come_back_time == 0)
    {
        x_val = 0;
        y_val += THREAT_GRAVITY_FALL;
        if(y_val >= MAX_FALL_SPEED)
        {
            y_val = MAX_FALL_SPEED;
        }

        if(input_type.left == 1)
        {
            x_val -= THREAT_SPEED;
        }
        else if(input_type.right == 1)
        {
            x_val += THREAT_SPEED;
        }
        CheckToMap(map_data);
    }
    else if(come_back_time > 0)
    {
        come_back_time--;
        if(come_back_time == 0)
        {
            InitThreats();
        }
    }
}

void ThreatObjects::InitThreats()
{
            x_val = 0;
            y_val = 0;
            if(x_pos > 256)
            {
                x_pos -= 256;
                animation_a -= 256;
                animation_b -= 256;
            }
            else
            {
                x_pos = 0;
            }
            y_pos = 0;
            come_back_time = 0;
            input_type.left = 1;
}

void ThreatObjects::CheckToMap(Map& map_data)
{
    int x1 = 0;
    int x2 = 0;

    int y1 = 0;
    int y2 = 0;

    //horizontally
    int height_min = height_frame < TILE_SIZE ? height_frame : TILE_SIZE;

    x1 = (x_pos + x_val)/TILE_SIZE;
    x2 = (x_pos + x_val + width_frame - 1)/TILE_SIZE;

    y1 = (y_pos)/TILE_SIZE;
    y2 = (y_pos + height_min -1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_X && y1 >= 0 && y2 < MAX_Y)
    {
        if(x_val > 0) //moing right
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];
            if((val1 != 0 && val1 != MONEY)|| (val2 != 0 && val2 != MONEY))
            {
                x_pos = x2*TILE_SIZE;
                x_pos -= width_frame + 1;
                x_val = 0;
            }
        }
        else if(x_val < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];
            if((val1 != 0 && val1 != MONEY) || (val2 != 0 && val2 != MONEY))
            {
                x_pos = (x1 + 1)*TILE_SIZE;
                x_val = 0;
            }
        }
    }

    //vertically
    int width_min = width_frame < TILE_SIZE ? width_frame :TILE_SIZE;
    x1 = (x_pos)/TILE_SIZE;
    x2 = (x_pos + width_min)/TILE_SIZE;

    y1 = (y_pos + y_val)/TILE_SIZE;
    y2 = (y_pos + y_val + height_frame -1)/TILE_SIZE;

    if(x1 >= 0 && x2 < MAX_X && y1 >= 0 && y2 < MAX_Y)
    {
        if(y_val > 0)
        {
            int val1 = map_data.tile[y2][x1];
            int val2 = map_data.tile[y2][x2];
            if((val1 != 0 && val1 != MONEY) || (val2 != 0 && val2 != MONEY))
            {
                y_pos = y2*TILE_SIZE;
                y_pos -= (height_frame +1);
                y_val = 0;
                isgrounded =  true;
            }
        }
        else if(y_val < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if((val1 != 0 && val1 != MONEY)|| (val2 != 0 && val2 != MONEY))
            {
                y_pos = (y1 + 1)*TILE_SIZE;
                y_val = 0;
            }
        }
    }

    x_pos += x_val;
    y_pos += y_val;

    if(x_pos < 0)
    {
        x_pos = 0;
    }
    else if(x_pos + width_frame > map_data.max_x)
    {
        x_pos = map_data.max_x - width_frame -1;
    }
    if(y_pos > map_data.max_y)
    {
        come_back_time  = 60;
    }
}



void ThreatObjects::implementmovetype(SDL_Renderer* screen)
{
    if(type_move == STATIC_THREAT)
    {
        ;
    }
    else
    {
        if(isgrounded == true)
        {
            if(x_pos > animation_b)
            {
                input_type.left = 1;
                input_type.right = 0;
                LoadImg("assets/threat_left.png", screen);
            }
            else if(x_pos < animation_a)
            {
                input_type.left = 0;
                input_type.right = 1;
                LoadImg("assets/threat_right.png", screen);
            }
        }
        else
        {
            if(input_type.left == 1)
            {
                LoadImg("assets/threat_left.png", screen);
            }
        }
    }
}

void ThreatObjects::InitSpell(SpellObject* p_spell, SDL_Renderer* screen)
{
    if(p_spell != NULL)
    {
        p_spell->set_spell_type(SpellObject::RINGED);
        p_spell->LoadImgSpell(screen);
        p_spell->set_ismoving(true);
        p_spell->set_spell_direction(SpellObject::DIR_LEFT);
        p_spell->Set_x_val(15);
        p_spell->SetRect(m_rect.x +10, y_pos + 10);
        spell_list.push_back(p_spell);
    }
}

void ThreatObjects::CreateSpell(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
    for(int i = 0; i < spell_list.size(); i++)
    {
        SpellObject* p_spell = spell_list.at(i);
        if(p_spell != NULL)
        {
            if(p_spell->get_ismoving())
            {
                int spell_distance = m_rect.x + width_frame - p_spell->GetRect().x;
                if(spell_distance < 300 && spell_distance > 0)
                {
                    p_spell->HandleMove(x_limit, y_limit);
                    p_spell->Render(screen);
                }
                else{
                    p_spell->set_ismoving(false);
                }

            }
            else
            {
                p_spell->set_ismoving(true);
                p_spell->SetRect(m_rect.x + 10, y_pos + 10);
            }
        }
    }
}

void ThreatObjects::RemoveSpell(const int& indx)
{
    int size = spell_list.size();
    if(size > 0 && indx < size)
    {
        SpellObject* p_spell = spell_list.at(indx);
        spell_list.erase(spell_list.begin() + indx);
        if(p_spell)
        {
            delete p_spell;
            p_spell = NULL;
        }
    }
}




