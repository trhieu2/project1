#ifndef WARRIOR_H
#define WARRIOR_H

#include "SDL_mixer.h"

#include "SpellObject.h"
#include "GameFunction.h"
#include "BaseObject.h"
#include <vector>

#define GRAVITY 0.8
#define MAX_GRAVITY 9.8
#define WARRIOR_SPEED 6
#define WARRIOR_JUMPFORCE 20

class Warrior : public BaseObject
{
    public:
        Warrior();
        ~Warrior();

        enum WalkType
        {
            WALK_NONE = 0,
            WALK_RIGHT = 1,
            WALK_LEFT = 2,
        };
        bool LoadImg(std::string path, SDL_Renderer* screen);
        void Show(SDL_Renderer* des);
        void HandleInput(SDL_Event events, SDL_Renderer* screen);
        void set_clip();

        void DoPlayer(Map& map_data);
        void CheckToMap(Map& map_data);
        void SetMapXY(const int map_x, const int map_y) {map_x_ = map_x; map_y_ = map_y;}
        void Center(Map& map_data);
        void UpdateImngWarrior(SDL_Renderer* des);
        SDL_Rect GetRectFrame();
        void set_spell_list(std::vector<SpellObject*> spell_list)
        {
            p_spell_list = spell_list;
        }
        std::vector<SpellObject*> get_spell_list() const {return p_spell_list;}
        void HandleSpell(SDL_Renderer* des);
        void RemoveSpell(const int& indx);
        void IncreaseMoney();
        void set_comeback_time(const int& cbtime){come_back_time = cbtime;}
        int GetMoneyCount() const {return money_count;}
        void Reset()
        {
            x_pos = 0;
            money_count = 0;
        }
    private:
        int money_count ;
        std::vector<SpellObject*> p_spell_list;
        float x_val;
        float y_val;

        float x_pos;
        float y_pos;

        int width_frame;
        int height_frame;

        SDL_Rect frame_clip_[3];
        Input input_type;
        int frame;
        int status;

        bool isgrounded;

        int map_x_;
        int map_y_;

        int come_back_time;

        Mix_Chunk* jumpSound;
};

#endif // WARRIOR_H
