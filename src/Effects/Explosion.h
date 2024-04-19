#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "GameFunction.h"
#include "BaseObject.h"

#define NUM_FRAME_EXP 8

class Explosion : public BaseObject
{
    public:
        Explosion();
        ~Explosion();
    public:
        void set_clip();
        void set_frame(const int& fr) {frame = fr;}
        virtual bool LoadImg(std::string path, SDL_Renderer* screen);
        void Show(SDL_Renderer* screen);
        int get_frame_width() const {return frame_width;}
        int get_frame_height() const {return frame_height;}
    private:
        int frame_width;
        int frame_height;

        unsigned int frame;
        SDL_Rect frame_clip[NUM_FRAME_EXP];
};

#endif // EXPLOSION_H
