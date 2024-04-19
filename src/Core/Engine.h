#ifndef ENGINE_H
#define ENGINE_H

#include"SDL.h"
#include"SDL_image.h"
#include "GameMap.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 600

class Engine{
public:
    static Engine* GetInstance(){
        return s_Instance = (s_Instance != nullptr) ? s_Instance : new Engine();
    }
    bool Init();
    bool Clean();
    bool Quit();

    void Update();
    void Render();
    void Events();

    inline GameMap* GetMap(){return m_LevelMap;}
    inline bool IsRunning(){return m_IsRunning;}
    inline SDL_Renderer* GetRenderer(){return m_Renderer;}

private:
    Engine(){}
    static Engine* s_Instance;
    bool m_IsRunning;
    GameMap* m_LevelMap;
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
};

#endif

