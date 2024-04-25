#include "GameFunction.h"
#include "TextManager.h"

#include <math.h>

GameFunction::GameFunction()
{
    //ctor
}

GameFunction::~GameFunction()
{
    //dtor
}

bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
    Mix_Chunk* collideSound = Mix_LoadWAV("music/collide.wav");

    int centerX1 = object1.x + object1.w / 2;
    int centerY1 = object1.y + object1.h / 2;
    int centerX2 = object2.x + object2.w / 2;
    int centerY2 = object2.y + object2.h / 2;

    int distance = sqrt(pow(centerX2 - centerX1, 2) + pow(centerY2 - centerY1, 2));

    int box1Size = object1.w / 2;
    int box2Size = object2.w / 2;

    int minDistance = (box1Size + box2Size) / 2;

    if (distance < minDistance)
    {
        Mix_PlayChannel(-1, collideSound, 0);
        return true;
    }
    else
    {
        return false;
    }
}


