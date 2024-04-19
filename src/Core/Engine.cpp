#include "Engine.h"
#include "TextureManager.h"
#include "Input.h"

#include"Warrior.h"
#include"SDL.h"
#include"Timer.h"
#include"MapParser.h"
#include<iostream>
#include "Camera.h"

Engine* Engine::s_Instance = nullptr;
Warrior* player = nullptr;

bool Engine::Init()
{
    if(SDL_Init(SDL_INIT_VIDEO)!=0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0)
    {
        SDL_Log("Failed to initialize SDL: %s",SDL_GetError());
        return false;
    }
    m_Window = SDL_CreateWindow("MISSION CAT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,SCREEN_HEIGHT,0);
    if(m_Window == nullptr)
    {

        SDL_Log("Failed to create: %s", SDL_GetError());
        return false;
    }
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(m_Renderer ==  nullptr)
    {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;

    }
    if(!MapParser::GetInstance()->Load()){
        std::cout<<"Fail to Load map"<<std::endl;
        return false;
    }

    m_LevelMap = MapParser::GetInstance()->GetMap("MAP");

    TextureManager::GetInstance()->Load("player","assets/idle.png");
    TextureManager::GetInstance()->Load("player_run","assets/run.png");
    TextureManager::GetInstance()->Load("player_jump", "assets/jump.png");
    TextureManager::GetInstance()->Load("bg", "assets/images/bg.png");

    player = new Warrior(new Properties("player",100,200,128,96));

    Camera::GetInstance()->SetTarget(player->GetOrigin());
    return m_IsRunning = true;
}

bool Engine::Clean()
{
    TextureManager::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
}

bool Engine::Quit()
{
    m_IsRunning = false;
}

void Engine::Update()
{
    float dt = Timer::GetInstance()->GetDeltaTime();
    player->Update(dt);
    m_LevelMap->Update();
    Camera::GetInstance()->Update(dt);
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 124, 210, 254, 256);
    SDL_RenderClear(m_Renderer);

    TextureManager::GetInstance()->Draw("bg", 0, 0, 2100, 1050);
    m_LevelMap->Render();

    player->Draw();
    SDL_RenderPresent(m_Renderer);

}

void Engine::Events()
{
    Input::GetInstance()->Listen();
}


