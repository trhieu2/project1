#ifndef MONEY_H
#define MONEY_H

#include"GameFunction.h"
#include"BaseObject.h"

class Money : public BaseObject
{
    public:
        Money();
        ~Money();

        void Init(SDL_Renderer* screen);
        void Show(SDL_Renderer* screen);
        void SetPos(const int& x, const int& y)
        {
            x_pos = x; y_pos =y;
        }
    private:
        int x_pos;
        int y_pos;

};

#endif // MONEY_H
