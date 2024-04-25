#include "Warrior.h"
#include "BaseObject.h"

Warrior::Warrior()
{
    frame = 0;
    x_pos = 0;
    y_pos = 0;
    x_val = 0;
    width_frame = 0;
    height_frame = 0;
    status = WALK_NONE;
    input_type.left = 0;
    input_type.right = 0;
    input_type.jump = 0;
    input_type.down = 0;
    input_type.up = 0;
    isgrounded = false;
    map_x_ = 0;
    map_y_ = 0;
    come_back_time = 0;
    money_count = 0;

    jumpSound = Mix_LoadWAV("music/playerjump.wav");
    fallingSound = Mix_LoadWAV("music/losing_or_falling.wav");
}

Warrior::~Warrior()
{

}

bool Warrior::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if(ret == true)
    {
        width_frame = m_rect.w/3;
        height_frame = m_rect.h;
    }

    return ret;
}

SDL_Rect Warrior::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = m_rect.x;
    rect.y = m_rect.y;
    rect.w = width_frame;
    rect.h = height_frame;

    return rect;
}


void Warrior::set_clip()
{
    if(width_frame > 0 && height_frame >0)
    {
        for(int k = 0; k < 3; k++){
        frame_clip_[k].x = width_frame*k;
        frame_clip_[k].y = 0;
        frame_clip_[k].w = width_frame;
        frame_clip_[k].h = height_frame;
        }
    }
}


void Warrior::Show(SDL_Renderer* des)
{
    UpdateImngWarrior(des);

    if(status == WALK_LEFT){
        LoadImg("assets/player_left.png", des);
    }
    else{
        LoadImg("assets/player_right.png", des);
    }

    if(input_type.left == 1 || input_type.right == 1)
    {
        frame++;
    }
    else{
        frame = 0;
    }
    if(frame >= 3)
    {
        frame = 0;
    }
    if(come_back_time == 0)
    {
       m_rect.x = x_pos - map_x_;
       m_rect.y = y_pos - map_y_;

       SDL_Rect* current_clip = &frame_clip_[frame];

       SDL_Rect renderQuad = {m_rect.x, m_rect.y, width_frame, height_frame};

       SDL_RenderCopy(des, m_object, current_clip, &renderQuad);
    }
}

void Warrior::HandleInput(SDL_Event events, SDL_Renderer* screen)
{
    if(events.type == SDL_KEYDOWN)
    {
        switch(events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                status = WALK_RIGHT;
                input_type.right = 1;
                input_type.left = 0;
                UpdateImngWarrior(screen);
            }
            break;
        case SDLK_LEFT:
            {
                status = WALK_LEFT;
                input_type.left = 1;
                input_type.right = 0;
                UpdateImngWarrior(screen);
            }
            break;
        default:
            break;
        }
    }else if(events.type ==SDL_KEYUP)
    {
        switch(events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                input_type.right = 0;
            }
            break;
        case SDLK_LEFT:
            {
                input_type.left = 0;
            }
            break;
        default:
            break;
        }
    }
    if(events.type == SDL_MOUSEBUTTONDOWN)
    {
        if(events.button.button == SDL_BUTTON_RIGHT)
        {
            input_type.jump = 1;
            Mix_PlayChannel(-1, jumpSound, 0);
        }
        else if(events.button.button == SDL_BUTTON_LEFT)
        {
            SpellObject* p_spell = new SpellObject();
            p_spell->set_spell_type(SpellObject::NORING);
            p_spell->LoadImgSpell(screen);

            if(status == WALK_LEFT)
            {
                p_spell->set_spell_direction(SpellObject::DIR_LEFT);
                p_spell->SetRect(this->m_rect.x, m_rect.y + height_frame *0.25);
            }
            else
            {
                p_spell->set_spell_direction(SpellObject::DIR_RIGHT);
                p_spell->SetRect(this->m_rect.x + width_frame -20, m_rect.y + height_frame *0.25);
            }
            p_spell->Set_x_val(20);
            p_spell->Set_y_val(20);
            p_spell->set_ismoving(true);

            p_spell_list.push_back(p_spell);
        }
    }
}

void Warrior::HandleSpell(SDL_Renderer* des)
{
    for(int i = 0; i < p_spell_list.size(); i++)
    {
        SpellObject* p_spell = p_spell_list.at(i);
        if(p_spell  !=  NULL)
        {
            if(p_spell->get_ismoving() == true)
            {
                p_spell->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_spell->Render(des);
            }
            else
            {
                p_spell_list.erase(p_spell_list.begin() + i);
                if(p_spell != NULL)
                {
                    delete p_spell;
                    p_spell = NULL;
                }
            }
        }
    }
}

void Warrior::RemoveSpell(const int& indx)
{
    int size = p_spell_list.size();
    if(size > 0 && indx < size)
    {
        SpellObject* p_spell = p_spell_list.at(indx);
        p_spell_list.erase(p_spell_list.begin() + indx);
        if(p_spell)
        {
            delete p_spell;
            p_spell = NULL;
        }
    }
}

void Warrior::DoPlayer(Map& map_data)
{
    if(come_back_time == 0)
    {
        x_val = 0;
        y_val += 0.8;

    if(y_val >= MAX_GRAVITY)
    {
        y_val = MAX_GRAVITY;
    }

    if(input_type.left == 1)
    {
        x_val -= WARRIOR_SPEED;
    }
    else if(input_type.right == 1)
    {
        x_val += WARRIOR_SPEED;
    }

    if(input_type.jump == 1)
    {
        if(isgrounded == true){
        y_val = -WARRIOR_JUMPFORCE;
        }
        isgrounded = false;
        input_type.jump = 0;
    }
    CheckToMap(map_data);
    Center(map_data);
    }

    if(come_back_time > 0)
    {
        come_back_time--;
        if(come_back_time ==0)
        {
            isgrounded = false;
            if(x_pos > 256)
            {
                x_pos -= 256;
            }
            else{
                x_pos = 0;
            }
            y_pos = 0;
            x_val = 0;
            y_val = 0;
        }
    }
}

void Warrior::Center(Map& map_data)
{
    map_data.start_x = x_pos - (SCREEN_WIDTH/2);
    if(map_data.start_x < 0)
    {
        map_data.start_x = 0;
    }
    else if(map_data.start_x + SCREEN_WIDTH >= map_data.max_x)
    {
        map_data.start_x = map_data.max_x - SCREEN_WIDTH;
    }
    map_data.start_y = y_pos - (SCREEN_HEIGHT/2);
    if(map_data.start_y < 0)
    {
        map_data.start_y = 0;
    }
    else if(map_data.start_y = SCREEN_HEIGHT >= map_data.max_y)
    {
        map_data.start_y = map_data.max_y - SCREEN_HEIGHT;
    }
}

void Warrior::CheckToMap(Map& map_data)
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
        if(x_val > 0) //moving right
        {
            int val1 = map_data.tile[y1][x2];
            int val2 = map_data.tile[y2][x2];

            if(val1 == MONEY || val2 == MONEY)
            {
                map_data.tile[y1][x2] = 0;
                map_data.tile[y2][x2] = 0;
                IncreaseMoney();
            }
            else{
                if(val1 != 0 || val2 != 0)
            {
                x_pos = x2*TILE_SIZE;
                x_pos -= width_frame + 1;
                x_val = 0;
            }
            }
        }
        else if(x_val < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y2][x1];

            if(val1 == MONEY || val2 == MONEY)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y2][x1] = 0;
                IncreaseMoney();
            }
            else{
                if(val1 != 0 || val2 != 0)
            {
                x_pos = (x1 + 1)*TILE_SIZE;
                x_val = 0;
            }
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
            if(val1 == MONEY || val2 == MONEY)
            {
                map_data.tile[y2][x1] = 0;
                map_data.tile[y2][x2] = 0;
                IncreaseMoney();
            }
            else
            {
                if(val1 != 0 || val2 != 0)
            {
                y_pos = y2*TILE_SIZE;
                y_pos -= (height_frame +1);
                y_val = 0;
                isgrounded =  true;
                if(status == WALK_NONE)
                {
                    status = WALK_RIGHT;
                }
            }
            }
        }
        else if(y_val < 0)
        {
            int val1 = map_data.tile[y1][x1];
            int val2 = map_data.tile[y1][x2];
            if(val1 == MONEY || val2 == MONEY)
            {
                map_data.tile[y1][x1] = 0;
                map_data.tile[y1][x2] = 0;
                IncreaseMoney();
            }
            else
            {
                if(val1 != 0 || val2 != 0)
            {
                y_pos = (y1 + 1)*TILE_SIZE;
                y_val = 0;
            }
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
        Mix_PlayChannel(-1, fallingSound, 0);
        come_back_time  = 60;
    }
}

void Warrior::IncreaseMoney()
{
    money_count += 1000;
}

void Warrior::UpdateImngWarrior(SDL_Renderer* des)
{
    if(isgrounded == true)
    {
        if(status == WALK_LEFT)
        {
            LoadImg("assets/player_left.png", des);
        }
        else
        {
            LoadImg("assets/player_right.png",des);
        }
    }
    else
    {
        if(status == WALK_LEFT)
        {
            LoadImg("assets/jump_left.png", des);
        }
        else
        {
            LoadImg("assets/jump_right.ong", des);
        }
    }
}

