#ifndef BUTTON_H
#define BUTTON_H

#include "SDL.h"

#include <SDL2/SDL.h>

class Button {
private:
    SDL_Rect rect;
    SDL_Texture* texture;

public:
    Button(int x, int y, int w, int h, SDL_Texture* tex) : rect{x, y, w, h}, texture{tex} {}

    void render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    bool isClicked(int mouseX, int mouseY) {
        if (mouseX < rect.x || mouseX > rect.x + rect.w || mouseY < rect.y || mouseY > rect.y + rect.h) {
            return false;
        }
        return true;
    }
};



#endif // BUTTON_H
