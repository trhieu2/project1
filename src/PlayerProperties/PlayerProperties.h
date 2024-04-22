#ifndef PLAYERPROPERTIES_H
#define PLAYERPROPERTIES_H

#include "GameFunction.h"
#include "BaseObject.h"

class PlayerProperties : public BaseObject
{
    public:
        PlayerProperties();
        ~PlayerProperties();

        void SetNum(const int& num) {number_ = num;}
        void AddPos(const int& xPos);
        void Show(SDL_Renderer* screen);
        void Init(SDL_Renderer* screen);

        void Increase();
        void Decrease();

    private:
        int number_;
        std::vector<int> post_list;

};


#endif // PLAYERPROPERTIES_H
