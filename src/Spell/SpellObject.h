#ifndef SPELLOBJECT_H
#define SPELLOBJECT_H

#include "BaseObject.h"
#include"GameFunction.h"

class SpellObject : public BaseObject
{
    public:
        SpellObject();
        ~SpellObject();

        enum SPELLDIRECTION
        {
            DIR_RIGHT = 20,
            DIR_LEFT = 21,
            DIR_UP = 22,
            DIR_UP_LEFT = 23,
            DIR_UP_RIGHT = 24,
            DIR_DOWN = 25,
            DIR_DOWN_RIGHT = 26,
        };

        enum SpeelType
        {
            RINGED = 50,
            NORING = 51,
        };

        void Set_x_val(const int& xVal) {x_val = xVal;}
        void Set_y_val(const int& yVal) {y_val = yVal;}
        int get_x_val() const {return x_val;}
        int get_y_val() const {return y_val;}

        void set_ismoving(const bool& is_Moving){is_moving = is_Moving;}
        bool get_ismoving() const {return is_moving;}

        void HandleMove(const int& x_border, const int& y_border);

        void set_spell_direction(const unsigned int& spell_dir) {spell_direction = spell_dir;}
        unsigned int get_spell_direction() const {return spell_direction;}
        void set_spell_type(const unsigned int& spelltype){spell_type = spelltype;}
        unsigned int get_spell_type() const {return spell_type;}

        void LoadImgSpell(SDL_Renderer* des);
    private:
        int x_val;
        int y_val;
        bool is_moving;
        unsigned int spell_direction;
        unsigned int spell_type;

};

#endif // SPELLOBJECT_H
