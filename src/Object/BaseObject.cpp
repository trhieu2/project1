#include "BaseObject.h"

bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    Free();
    SDL_Texture* new_texture = NULL;

    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if(load_surface != NULL)
    {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if(new_texture != NULL)
        {
            m_rect.w = load_surface->w;
            m_rect.h = load_surface->h;
        }

        SDL_FreeSurface(load_surface);
    }
    m_object = new_texture;

    return m_object != NULL;
}

void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip)
{
    SDL_Rect renderquad = {m_rect.x, m_rect.y, m_rect.w, m_rect.h};

    SDL_RenderCopy(des, m_object, clip, &renderquad);
}

BaseObject::BaseObject()
{
    m_object = NULL;
    m_rect.x = 0;
    m_rect.y = 0;
    m_rect.w = 0;
    m_rect.h = 0;
}

BaseObject::~BaseObject()
{
    Free();
}

void BaseObject::Free()
{
    if(m_object != NULL)
    {
        SDL_DestroyTexture(m_object);
        m_object = NULL;
        m_rect.w = 0;
        m_rect.h = 0;
    }
}

