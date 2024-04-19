#include "Explosion.h"

Explosion::Explosion()
{
    frame_height = 0;
    frame_width = 0;
    frame = 0;
}

Explosion::~Explosion()
{

}

void Explosion::set_clip()
{
    if(frame_width > 0 && frame_height > 0)
    {
        for(int k = 0; k < NUM_FRAME_EXP ; k++)
        {
            frame_clip[k].x = k*frame_width;
            frame_clip[k].y = 0;
            frame_clip[k].w = frame_width;
            frame_clip[k].h = frame_height;
        }
    }
}

bool Explosion::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);

    if(ret)
    {
        frame_width = m_rect.w/NUM_FRAME_EXP;
        frame_height = m_rect.h;
    }
    return ret;
}

void Explosion::Show(SDL_Renderer* screen)
{
    SDL_Rect* current_clip = &frame_clip[frame];
    SDL_Rect render_quad = {m_rect.x, m_rect.y, frame_width, frame_height};
    if(current_clip != NULL)
    {
        render_quad.w = current_clip->w;
        render_quad.h = current_clip->h;
    }
    SDL_RenderCopy(screen, m_object, current_clip, &render_quad);
}
