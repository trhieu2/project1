#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>

class Button {
public:
    Button(int x, int y, int w, int h) : rect{x, y, w, h} {}

    bool isClicked(int mouseX, int mouseY) {
        if (mouseX > rect.x && mouseX < rect.x + rect.w &&
            mouseY > rect.y && mouseY < rect.y + rect.h) {
            return true;
        }
        return false;
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
        SDL_RenderFillRect(renderer, &rect);
    }

private:
    SDL_Rect rect;
};

#endif // BUTTON_H
