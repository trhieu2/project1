#ifndef CHERRY_H
#define CHERRY_H


#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

class Cherry {
public:
    Cherry(SDL_Renderer* renderer, const char* spriteSheetPath, int width, int height)
        : m_renderer(renderer), m_width(width), m_height(height), m_cherryTexture(nullptr) {
        m_cherryTexture = loadTexture(spriteSheetPath);
        if (m_cherryTexture == nullptr) {
            std::cerr << "Failed to load cherry sprite sheet texture!" << std::endl;
        }
        srand(time(nullptr)); // Seed random number generator
    }

    ~Cherry() {
        SDL_DestroyTexture(m_cherryTexture);
    }

    void render(int x, int y) {
        if (m_cherryTexture != nullptr) {
            SDL_Rect srcRect = { rand() % m_width, 0, m_width, m_height }; // Randomly select a frame
            SDL_Rect destRect = { x, y, m_width, m_height };
            SDL_RenderCopy(m_renderer, m_cherryTexture, &srcRect, &destRect);
        }
    }

private:
    SDL_Texture* loadTexture(const char* path) {
        SDL_Surface* surface = IMG_Load(path);
        if (surface == nullptr) {
            std::cerr << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
    }

    SDL_Renderer* m_renderer;
    int m_width;
    int m_height;
    SDL_Texture* m_cherryTexture;
};


#endif // CHERRY_H
