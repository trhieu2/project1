#include <iostream>
#include <string>
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
#include "Button.h"

BaseObject g_background;
BaseObject g_start_screen;

TTF_Font* font_time = NULL;

int high_score = 0;

void RenderHighScore(TextManager& text_manager, SDL_Renderer* renderer, TTF_Font* font)
{
    std::string score_text = "High Score: " + std::to_string(high_score);
    text_manager.SetText(score_text);
    text_manager.SetColor(TextManager::WHITE_TEXT);
    text_manager.LoadFromRenderText(font, renderer);
    text_manager.RenderTextt(renderer, SCREEN_WIDTH - 200, 50);
}

bool Init()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if(ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("MISSION CAT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
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
        font_time = TTF_OpenFont("font/dlxfont_.ttf", 12);
        if(font_time == NULL)
        {
            success = false;

        }
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
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

    for(int i = 0; i < 20; i++)
    {
        ThreatObjects* p_threat = new ThreatObjects();
        if(p_threat != NULL)
        {
            p_threat->LoadImg("assets/threat_left.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatObjects::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i * 300);
            p_threat->set_y_pos(200);

            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() + 60;
            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }
    }

    for(int i = 0; i < 20; i++)
    {
        ThreatObjects* p_threat = new ThreatObjects();
        if(p_threat != NULL)
        {
            p_threat->LoadImg("assets/threat_level.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(300 + i * 1000);
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

void CleanUp(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, TTF_Font* titleFont, Mix_Music* startMusic, Mix_Music* inGameMusic) {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    if (font) {
        TTF_CloseFont(font);
    }
    if (titleFont) {
        TTF_CloseFont(titleFont);
    }
    if (startMusic) {
        Mix_FreeMusic(startMusic);
    }
    if (inGameMusic) {
        Mix_FreeMusic(inGameMusic);
    }
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
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

    TTF_Font* font = TTF_OpenFont("font/kaph.ttf", 20);

    TTF_Font* title_font = TTF_OpenFont("font/kaph.ttf", 35);

    Button playButton(g_screen, "buttons/play1.png", "buttons/play2.png", 560, 200, 1);
    Button InformationButton(g_screen, "buttons/infor1.png", "buttons/infor2.png",560, 270, 1);
    Button soundButton(g_screen, "buttons/music1.png", "buttons/music2.png", 560, 340, 1);
    Button mutedSoundButton(g_screen, "buttons/music3.png", "buttons/music4.png", 560, 340, 1);
    Button backButton(g_screen, "buttons/back1.png", "buttons/back2.png", 50, 50, 1);

    //Loading map
    GmeMap game_map;
    game_map.LoadMap("map/map01.dat");
    game_map.LoadTiles(g_screen);

    //Loading Player
    Warrior p_player;
    p_player.LoadImg("assets/player_right.png", g_screen);
    p_player.set_clip();

    PlayerProperties player_props;
    player_props.Init(g_screen);

    //Loading money
    Money player_money;
    player_money.Init(g_screen);
    player_money.SetPos(SCREEN_WIDTH*0.5 - 300, 8);

    std::vector<ThreatObjects*> threats_list = CreateThreatObjects();

    Explosion exp_threat;
    bool tRect = exp_threat.LoadImg("assets/exp3.png", g_screen);
    if(!tRect) return -1;
    exp_threat.set_clip();

    int num_die = 0;

    //Game Tile Text
    TextManager title;
    title.SetText("MISSION CAT");
    title.SetColor(TextManager::RED_TEXT);
    title.LoadFromRenderText(title_font, g_screen);

    //Time Text
    TextManager time_game;
    time_game.SetColor(TextManager::WHITE_TEXT);

    TextManager mark_game;
    mark_game.SetColor(TextManager::WHITE_TEXT);
    UINT mark_value = 0;

    TextManager money_game;
    money_game.SetColor(TextManager::WHITE_TEXT);

     //Resume Text
    TextManager resume_text;
    resume_text.SetText("Press space to resume");
    resume_text.SetColor(TextManager::BLACK_TEXT);
    resume_text.LoadFromRenderText(font, g_screen);

    SDL_Init(SDL_INIT_AUDIO);
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    Mix_Music *startScreenMusic = Mix_LoadMUS("music/hurry_up_and_run.mp3");
    Mix_Music *inGameMusic = Mix_LoadMUS("music/dark-happy-world.mp3");
    Mix_Chunk * winningMusic = Mix_LoadWAV("music/winning.wav");

    if(startScreenMusic == NULL || inGameMusic == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    enum GameState{
        START_SCREEN,
        PLAYING,
        SHOWING_INSTRUCTIONS,
        BACK_TO_START,
    };

    GameState game_state = START_SCREEN;

    bool isMusicMuted = false;

    while (game_state != PLAYING) {
        background.Render(g_screen, NULL);
        if(!Mix_PlayingMusic())
        {
            Mix_PlayMusic(startScreenMusic, -1);
        }

        title.RenderTextt(g_screen, (SCREEN_WIDTH - title.GetWidth()) / 2, 50);

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool isplayHovered = playButton.isHovered(mouseX, mouseY);
        bool isinforHovered = InformationButton.isHovered(mouseX, mouseY);
        bool isMusicHovered = soundButton.isHovered(mouseX, mouseY);
        bool isMusicMutedHovered = mutedSoundButton.isHovered(mouseX, mouseY);
        bool isBackHovered = backButton.isHovered(mouseX, mouseY);

        if(game_state == START_SCREEN)
        {
            playButton.render(g_screen, isplayHovered);
            InformationButton.render(g_screen, isinforHovered);
            soundButton.render(g_screen, isMusicHovered);

            if(isMusicMuted)
            {
                mutedSoundButton.render(g_screen, mutedSoundButton.isHovered(mouseX, mouseY));
            }
        }
        else if(game_state == SHOWING_INSTRUCTIONS || game_state == BACK_TO_START)
        {
                  backButton.render(g_screen, isBackHovered);
                  TextManager instruc_text1;
                  instruc_text1.SetText("PRESS UP TO JUMP");

                  instruc_text1.SetColor(TextManager::WHITE_TEXT);
                  instruc_text1.LoadFromRenderText(font, g_screen);
                  int instruc_text1_width = instruc_text1.GetWidth();
                  int instruc_text1_height = instruc_text1.GetHeight();
                  int instruc_text1_x = (SCREEN_WIDTH - instruc_text1_width) / 2;
                  int instruc_text1_y = (SCREEN_HEIGHT - instruc_text1_height) / 2;
                  instruc_text1.RenderTextt(g_screen, instruc_text1_x, instruc_text1_y - 140);

                  TextManager instruc_text2;
                  instruc_text2.SetText("PRESS DOWN TO GET DOWN");

                  instruc_text2.SetColor(TextManager::BLACK_TEXT);
                  instruc_text2.LoadFromRenderText(font, g_screen);
                  int instruc_text2_width = instruc_text2.GetWidth();
                  int instruc_text2_height = instruc_text2.GetHeight();
                  int instruc_text2_x = (SCREEN_WIDTH - instruc_text2_width) / 2;
                  int instruc_text2_y = (SCREEN_HEIGHT - instruc_text2_height) / 2;
                  instruc_text2.RenderTextt(g_screen, instruc_text2_x, instruc_text2_y - 70);

                  TextManager instruc_text3;
                  instruc_text3.SetText("PRESS ENTER TO PAUSE THE GAME");

                  instruc_text3.SetColor(TextManager::BLACK_TEXT);
                  instruc_text3.LoadFromRenderText(font, g_screen);
                  int instruc_text3_width = instruc_text3.GetWidth();
                  int instruc_text3_height = instruc_text3.GetHeight();
                  int instruc_text3_x = (SCREEN_WIDTH - instruc_text3_width) / 2;
                  int instruc_text3_y = (SCREEN_HEIGHT - instruc_text3_height) / 2;
                  instruc_text3.RenderTextt(g_screen, instruc_text3_x, instruc_text3_y);

                  TextManager instruc_text4;
                  instruc_text4.SetText("PRESS ESC TO QUIT");

                  instruc_text4.SetColor(TextManager::BLACK_TEXT);
                  instruc_text4.LoadFromRenderText(font, g_screen);
                  int instruc_text4_width = instruc_text4.GetWidth();
                  int instruc_text4_height = instruc_text4.GetHeight();
                  int instruc_text4_x = (SCREEN_WIDTH - instruc_text4_width) / 2;
                  int instruc_text4_y = (SCREEN_HEIGHT - instruc_text4_height) / 2;
                  instruc_text4.RenderTextt(g_screen, instruc_text4_x, instruc_text4_y + 70);

                  TextManager instruc_text5;
                  instruc_text5.SetText("PRESS LEFT/RIGHT TO MOVE LEFT/RIGHT");

                  instruc_text5.SetColor(TextManager::BLACK_TEXT);
                  instruc_text5.LoadFromRenderText(font, g_screen);
                  int instruc_text5_width = instruc_text5.GetWidth();
                  int instruc_text5_height = instruc_text5.GetHeight();
                  int instruc_text5_x = (SCREEN_WIDTH - instruc_text5_width) / 2;
                  int instruc_text5_y = (SCREEN_HEIGHT - instruc_text5_height) / 2;
                  instruc_text5.RenderTextt(g_screen, instruc_text5_x, instruc_text5_y + 140);

                  TextManager instruc_text6;
                  instruc_text6.SetText("RIGHT CLICK TO ATTACK");

                  instruc_text6.SetColor(TextManager::BLACK_TEXT);
                  instruc_text6.LoadFromRenderText(font, g_screen);
                  int instruc_text6_width = instruc_text6.GetWidth();
                  int instruc_text6_height = instruc_text6.GetHeight();
                  int instruc_text6_x = (SCREEN_WIDTH - instruc_text6_width) / 2;
                  int instruc_text6_y = (SCREEN_HEIGHT - instruc_text6_height) / 2;
                  instruc_text6.RenderTextt(g_screen, instruc_text6_x, instruc_text6_y + 210);

        }
        SDL_RenderPresent(g_screen);

        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT) {
                game_state = PLAYING;
            }
            else if (g_event.type == SDL_KEYDOWN && g_event.key.keysym.sym == SDLK_RETURN) {
                game_state = PLAYING;
            }
            else if(g_event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x,y;
                SDL_GetMouseState(&x, &y);
                if(playButton.isClicked(x, y) && game_state == START_SCREEN)
                {
                    game_state = PLAYING;
                }
                if(InformationButton.isClicked(x, y) && game_state == START_SCREEN)
                {
                    game_state = SHOWING_INSTRUCTIONS;
                }
                if(backButton.isClicked(x,y) && (game_state == SHOWING_INSTRUCTIONS))
                {
                    game_state = START_SCREEN;
                }
                if(soundButton.isClicked(x,y) && game_state == START_SCREEN)
                {
                    isMusicMuted = !isMusicMuted;
                    if(isMusicMuted)
                    {
                        Mix_PauseMusic();
                    }
                    else
                    {
                        Mix_ResumeMusic();
                    }
                }
            }
        }
    }
    //Game Starts
    Mix_HaltMusic();

    SDL_RenderClear(g_screen);

    bool is_quit = false;
    bool is_paused = false;
    bool ingamemusic_playing = false;
    bool is_restart = false;

    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(inGameMusic,-1);
        ingamemusic_playing = true;
    }

    while(is_quit == false)
    {
        fps_timer.start();

        if(isMusicMuted)
        {
            Mix_HaltMusic();
        }
        while(SDL_PollEvent(&g_event) !=0)
              {
                  if(g_event.type == SDL_QUIT)
                {
                    is_quit = true;
                }
                else if(g_event.type == SDL_KEYDOWN)
                {
                    if(g_event.key.keysym.sym == SDLK_SPACE)
                    {
                        is_paused = !is_paused;
                        if(is_paused)
                        {
                            if(Mix_PlayingMusic() != 0)
                            {
                            Mix_HaltMusic();
                            ingamemusic_playing = false;
                            }
                        }
                        else
                        {
                            if(Mix_PlayingMusic() == 0 &&!isMusicMuted)
                            {
                            Mix_PlayMusic(inGameMusic, -1);
                            ingamemusic_playing = true;
                            }
                        }
                    }
                }
                else if(g_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    is_quit = true;
                }
                p_player.HandleInput(g_event, g_screen);
              }
              if (is_paused)
              {
                    int textWidth = resume_text.GetWidth();
                    int textHeight = resume_text.GetHeight();
                    int textX = (SCREEN_WIDTH - textWidth) / 2;
                    int textY = (SCREEN_HEIGHT - textHeight) / 2;

                    resume_text.RenderTextt(g_screen, textX, textY);
                    SDL_RenderPresent(g_screen);
                    continue;
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
                      if(bCol2 || bCol1)
                      {

                          num_die++;
                          if(num_die <= 3)
                          {
                              p_player.SetRect(0,0);
                              p_player.set_comeback_time(20);
                              SDL_Delay(500);
                              player_props.Decrease();
                              player_props.Render(g_screen);
                              continue;
                          }
                          else if(num_die > 3)
                          {
                          if(MessageBox(NULL, "PLAY AGAIN ?", "Info", MB_OKCANCEL | MB_ICONSTOP) == IDCANCEL)
                        {
                            is_quit = true;
                        }
                        else
                        {
                            num_die = 0;
                            is_restart = true;
                        }
                          }
                      }
                  }
              }

              int money_count = p_player.GetMoneyCount();
              std::string money_str = std::to_string(money_count);

              money_game.SetText(money_str);
              money_game.LoadFromRenderText(font_time, g_screen);
              money_game.RenderTextt(g_screen, SCREEN_WIDTH*0.5 - 250, 15);

              if(is_restart)
              {
                  p_player.Reset();
                  is_restart = false;
                  num_die = 3;
                  mark_value = 0;
                  money_count = 0;

                  std::string val_str_mark = std::to_string(mark_value);
                  std::string strMark("Mark: ");
                  strMark += val_str_mark;
                  mark_game.SetText(strMark);
                  mark_game.LoadFromRenderText(font_time, g_screen);
                  mark_game.RenderTextt(g_screen, SCREEN_WIDTH*0.5 - 50, 15);

                  std::string money_str = std::to_string(money_count);
                  money_game.SetText(money_str);
                  money_game.LoadFromRenderText(font_time, g_screen);
                  money_game.RenderTextt(g_screen, SCREEN_WIDTH*0.5 - 250, 15);

              }

              if(p_player.IsGameWon())
              {
                  Mix_PlayChannel(-1, winningMusic, 0);
                  TextManager win_text;
                  win_text.SetText("YOU WIN");
                  win_text.SetColor(TextManager::BLACK_TEXT);
                  win_text.LoadFromRenderText(font, g_screen);
                  int win_text_width = win_text.GetWidth();
                  int win_text_height = win_text.GetHeight();
                  int win_text_x = (SCREEN_WIDTH - win_text_width) / 2;
                  int win_text_y = (SCREEN_HEIGHT - win_text_height) / 2;
                  win_text.RenderTextt(g_screen, win_text_x, win_text_y);
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

              if(mark_value > high_score)
              {
                  high_score = mark_value;
              }
              RenderHighScore(mark_game, g_screen, font_time);

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

    for (int i = 0; i < threats_list.size(); ++i) {
    delete threats_list[i];
}

    threats_list.clear();
    Close();
    CleanUp(g_window, g_screen, font, title_font, startScreenMusic, inGameMusic);
    return 0;
}
