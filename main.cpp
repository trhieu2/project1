#include <iostream>
#include "GameFunction.h"
#include "BaseObject.h"
#include "GmeMap.h"
#include "Warrior.h"
#include "Timer.h"
#include "ThreatObjects.h"
#include "Explosion.h"
#include "TextManager.h"
#include "PlayerProperties.h"
#include "Money.h"

BaseObject g_background;
BaseObject g_start_screen;

TTF_Font* font_time = NULL;

bool Init()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("MISSION CAT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(g_window == NULL)
    {
        success = false;
    }
    else{
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if(g_screen == NULL)
            success = false;
        else{
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) && imgFlags))
               {
                   success = false;
               }
        }

        if(TTF_Init() == -1)
        {
            success = false;
        }
        font_time = TTF_OpenFont("font/dlxfont_.ttf", 15);
        if(font_time == NULL)
        {
            success = false;

        }
    }
    return success;
}

bool LoadBackGround()
{
    bool ret = g_background.LoadImg("assets/background.png", g_screen);
    if(ret== false)
    {
        return false;
    }
    return true;
}

void Close()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();

}

std::vector<ThreatObjects*> CreateThreatObjects()
{
    std::vector<ThreatObjects*> list_threats;

    ThreatObjects* dynamic_threats = new ThreatObjects[20];
    for(int i = 0; i < 20; i++)
    {
        ThreatObjects* p_threat = (dynamic_threats + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("assets/threat_left.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatObjects::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i*500);
            p_threat->set_y_pos(200);

            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() + 60;
            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }

    ThreatObjects* threats_objs = new ThreatObjects[20];

    for(int i = 0; i < 20; i++)
    {
        ThreatObjects* p_threat  = (threats_objs + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("assets/threat_level.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(300 + i*1200);
            p_threat->set_y_pos(250);
            p_threat->set_type_move(ThreatObjects::STATIC_THREAT);
            p_threat->set_input_left(0);

            SpellObject* p_spell = new SpellObject();
            p_threat->InitSpell(p_spell, g_screen);


            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}

int main(int argc, char* argv[])
{
    Timer fps_timer;

    if(Init() == false)
        return -1;
    if(LoadBackGround() == false)
        return -1;

    BaseObject background;
    if (background.LoadImg("assets/start_screen.png", g_screen) == false)
        return -1;

    TTF_Font* font = TTF_OpenFont("font/dlxfont_.ttf", 24);

    GmeMap game_map;
    game_map.LoadMap("map/map01.dat");
    game_map.LoadTiles(g_screen);

    Warrior p_player;
    p_player.LoadImg("assets/player_right.png", g_screen);
    p_player.set_clip();

    PlayerProperties player_props;
    player_props.Init(g_screen);

    Money player_money;
    player_money.Init(g_screen);
    player_money.SetPos(SCREEN_WIDTH*0.5 - 300, 8);

    std::vector<ThreatObjects*> threats_list = CreateThreatObjects();

    Explosion exp_threat;
    bool tRect = exp_threat.LoadImg("assets/exp3.png", g_screen);
    if(!tRect) return -1;
    exp_threat.set_clip();

    int num_die = 0;

    //Time Text
    TextManager time_game;
    time_game.SetColor(TextManager::WHITE_TEXT);

    TextManager mark_game;
    mark_game.SetColor(TextManager::WHITE_TEXT);
    UINT mark_value = 0;

    TextManager money_game;
    money_game.SetColor(TextManager::WHITE_TEXT);

    TextManager click_text;
    click_text.SetText("Press Enter to Play");
    click_text.SetColor(TextManager::WHITE_TEXT);
    click_text.LoadFromRenderText(font, g_screen);

    int textWidth = click_text.GetWidth();
    int textHeight = click_text.GetHeight();
    int textX = (SCREEN_WIDTH - textWidth) / 2;
    int textY = (SCREEN_HEIGHT - textHeight) / 2;

    click_text.RenderTextt(g_screen, textX, textY);
    SDL_RenderPresent(g_screen);

    bool start_game = false;
    while (!start_game) {
        background.Render(g_screen, NULL);

        click_text.RenderTextt(g_screen, textX, textY);
        SDL_RenderPresent(g_screen);

        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT) {
                start_game = true;
            }
            else if (g_event.type == SDL_KEYDOWN && g_event.key.keysym.sym == SDLK_RETURN) {
                start_game = true;
            }
        }
    }

    SDL_RenderClear(g_screen);

    bool is_quit = false;
    while(!is_quit)
    {
        fps_timer.start();
        while(SDL_PollEvent(&g_event) !=0)
              {
                  if(g_event.type == SDL_QUIT)
                {
                    is_quit = true;
                }

                p_player.HandleInput(g_event, g_screen);
              }
              SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
              SDL_RenderClear(g_screen);

              g_background.Render(g_screen, NULL);
              game_map.FillMap(g_screen);

              Map map_data = game_map.GetMap();

              p_player.HandleSpell(g_screen);
              p_player.SetMapXY(map_data.start_x, map_data.start_y);
              p_player.DoPlayer(map_data);
              p_player.Show(g_screen);

              game_map.SetMap(map_data);
              game_map.FillMap(g_screen);

              player_props.Show(g_screen);
              player_money.Show(g_screen);

              for(int i = 0; i < threats_list.size(); i++)
              {
                  ThreatObjects* p_threat = threats_list.at(i);
                  if(p_threat != NULL)
                  {
                      p_threat->SetMapXY(map_data.start_x, map_data.start_y);
                      p_threat->implementmovetype(g_screen);
                      p_threat->DoPlayer(map_data);

                      p_threat->CreateSpell(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                      p_threat->Show(g_screen);

                      SDL_Rect rect_player = p_player.GetRectFrame();
                      bool bCol1 = false;
                      std::vector<SpellObject*> tSpell_list = p_threat->get_spell_list();
                      for(int jj = 0; jj < tSpell_list.size(); jj++)
                      {
                          SpellObject* pt_spell = tSpell_list.at(jj);
                          if(pt_spell)
                          {
                              bCol1 = SDLCommonFunc::CheckCollision(pt_spell->GetRect(), rect_player);
                              if(bCol1)
                              {
                                  p_threat->RemoveSpell(jj);
                                  break;
                              }
                          }
                      }

                      SDL_Rect rect_threat = p_threat->GetRectFrame();
                      bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
                      if(bCol1 || bCol2)
                      {
                          if(num_die <= 3)
                          {
                              p_player.SetRect(0,0);
                              p_player.set_comeback_time(60);
                              SDL_Delay(1000);
                              player_props.Decrease();
                              player_props.Render(g_screen);
                              continue;
                          }
                          else
                          {
                              if(MessageBox(NULL, "GAME OVER", "Info", MB_OK | MB_ICONSTOP) == IDOK)
                          {
                              p_threat->Free();
                              Close();
                              SDL_Quit();
                              return 0;
                          }
                          }
                      }

                  }
              }
              int frame_exp_width = exp_threat.get_frame_width();
              int frame_exp_height = exp_threat.get_frame_height();

              std::vector<SpellObject*> spell_arr = p_player.get_spell_list();
              for(int r = 0; r < spell_arr.size(); r++)
              {
                  SpellObject* p_spell = spell_arr.at(r);
                  if(p_spell != NULL)
                  {
                      for(int t = 0; t < threats_list.size(); t++)
                      {
                          ThreatObjects* obj_threat = threats_list.at(t);
                          if(obj_threat != NULL)
                          {
                              SDL_Rect tRect;
                              tRect.x = obj_threat->GetRect().x;
                              tRect.y = obj_threat->GetRect().y;
                              tRect.w = obj_threat->get_width_frame();
                              tRect.h = obj_threat->get_height_frame();

                              SDL_Rect bRect = p_spell->GetRect();

                              bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);

                              if(bCol)
                              {
                                  mark_value += 100;
                                  for(int ex = 0; ex < NUM_FRAME_EXP; ex++)
                                  {
                                      int x_pos = p_spell->GetRect().x; - frame_exp_width*0.5;
                                      int y_pos = p_spell->GetRect().y; - frame_exp_height*0.5;

                                      exp_threat.set_frame(ex);
                                      exp_threat.SetRect(x_pos, y_pos);
                                      exp_threat.Show(g_screen);
                                  }

                                  p_player.RemoveSpell(r);
                                  obj_threat->Free();
                                  threats_list.erase(threats_list.begin() + t);
                              }
                          }
                      }
                  }
              }

              //Show game time
              std::string str_time = "Time: ";
              Uint32 time_val = SDL_GetTicks()/1000;
              Uint32 val_time = 300 - time_val;
              if(val_time <= 0)
              {
                  if(MessageBox(NULL, "GAME OVER", "Info", MB_OK | MB_ICONSTOP) == IDOK)
                  {
                      is_quit = true;
                      break;
                  }
              }
              else{
                std::string str_val = std::to_string(val_time);
                str_time = str_val;

                time_game.SetText(str_time);
                time_game.LoadFromRenderText(font_time, g_screen);
                time_game.RenderTextt(g_screen, SCREEN_WIDTH - 200, 15);
              }

              std::string val_str_mark = std::to_string(mark_value);
              std::string strMark("Mark: ");
              strMark += val_str_mark;

              mark_game.SetText(strMark);
              mark_game.LoadFromRenderText(font_time, g_screen);
              mark_game.RenderTextt(g_screen, SCREEN_WIDTH*0.5 - 50, 15);

              int money_count = p_player.GetMoneyCount();
              std::string money_str = std::to_string(money_count);

              money_game.SetText(money_str);
              money_game.LoadFromRenderText(font_time, g_screen);
              money_game.RenderTextt(g_screen, SCREEN_WIDTH*0.5 - 250, 15);

              SDL_RenderPresent(g_screen);

              int real_imp_time = fps_timer.get_ticks();
              int time_one_frame = 1000/FPS; //ms
              if(real_imp_time < time_one_frame)
              {
                  int delay_time = time_one_frame - real_imp_time;
                  if(delay_time >= 0)
                  {
                      SDL_Delay(delay_time);
                  }
              }
    }

    for(int i = 0; i < threats_list.size(); i++)
    {
        ThreatObjects* p_threat = threats_list.at(i);
        if(p_threat)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }

    threats_list.clear();
    Close();
    return 0;
}

