#ifndef THREATOBJECTS_H
#define THREATOBJECTS_H

#include "GameFunction.h"
#include "BaseObject.h"
#include "SpellObject.h"

#define THREAT_FRAME_NUM 3
#define THREAT_GRAVITY_FALL 0.8
#define MAX_FALL_SPEED 10
#define THREAT_SPEED 3

class ThreatObjects : public BaseObject
{
    public:
        ThreatObjects();
        ~ThreatObjects();

        enum TypeMove
        {
            STATIC_THREAT =0,
            MOVE_IN_SPACE_THREAT = 1,
        };

        void set_x_val(const float& xval){x_val = xval;}
        void set_y_val(const float& yval){y_val = yval;}

        void set_x_pos(const float& xp){x_pos = xp;}
        void set_y_pos(const float& yp){y_pos = yp;}
        float get_x_pos() const {return x_pos;}
        float get_y_pos() const {return y_pos;}
        void SetMapXY(const int& mp_x, const int& mp_y) {map_x = mp_x; map_y = mp_y;}

        void set_clips();
        bool LoadImg(std::string path, SDL_Renderer* screen);
        void Show(SDL_Renderer* des);
        int get_width_frame()const {return width_frame;}
        int get_height_frame()const {return height_frame;}

        void DoPlayer(Map& map_data);
        void CheckToMap(Map& mao_data);

        void InitThreats();
        void set_type_move (const int& typeMove) {type_move = typeMove;}
        void SetAnimationPos(const int& pos_a, const int& pos_b) {animation_a = pos_a; animation_b = pos_b;}
        void set_input_left(const int& ipleft){input_type.left = ipleft;}
        void implementmovetype(SDL_Renderer* screen);
        SDL_Rect GetRectFrame();

        std::vector<SpellObject*> get_spell_list() const {return spell_list;}
        void set_spell_list(const std::vector<SpellObject*>& sl_list){spell_list = sl_list;}
        void InitSpell(SpellObject* p_spell, SDL_Renderer* screen);
        void CreateSpell(SDL_Renderer* screen, const int& x_list, const int& y_list);
        void RemoveSpell(const int& indx);
    private:
        int frame;
        int width_frame;
        int height_frame;
        SDL_Rect frame_clip_[THREAT_FRAME_NUM];
        bool isgrounded;
        int come_back_time;
        float x_pos;
        float y_pos;
        float x_val;
        float y_val;
        int map_x;
        int map_y;

        int type_move;
        int animation_a;
        int animation_b;
        Input input_type;

        std::vector<SpellObject*> spell_list;
};

#endif // THREATOBJECTS_H
