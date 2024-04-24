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
    // Calculate the center coordinates of each object
    int centerX1 = object1.x + object1.w / 2;
    int centerY1 = object1.y + object1.h / 2;
    int centerX2 = object2.x + object2.w / 2;
    int centerY2 = object2.y + object2.h / 2;

    // Calculate the distance between the centers of the objects
    int distance = sqrt(pow(centerX2 - centerX1, 2) + pow(centerY2 - centerY1, 2));

    // Calculate the size of the collision box for each object (half of the object's width)
    int box1Size = object1.w / 2;
    int box2Size = object2.w / 2;

    // Calculate the minimum distance for collision (sum of the radii)
    int minDistance = (box1Size + box2Size) / 2;

    // Check if the distance is less than the minimum distance for collision
    if (distance < minDistance)
    {
        // Collision detected
        return true;
    }
    else
    {
        // No collision
        return false;
    }
}


