#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include "GameFunction.h"

class BaseObject
{
    public:
        BaseObject();
        ~BaseObject();
        void SetRect(const int& x, const int& y){m_rect.x = x, m_rect.y = y;}
        SDL_Rect GetRect()const {return m_rect;}
        SDL_Texture* GetObject() const {return m_object;}

        virtual bool LoadImg(std::string path, SDL_Renderer* screen);
        void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
        void Free();

    protected:
        SDL_Texture* m_object;
        SDL_Rect m_rect;
};

#endif // BASEOBJECT_H
