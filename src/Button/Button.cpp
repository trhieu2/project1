#include "Button.h"

Button::~Button() {}

void Button::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool inside = (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                       mouseY >= rect.y && mouseY <= rect.y + rect.h);
        if (inside) {
            clicked = true;
        }
    }
}

void Button::render() {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawRect(renderer, &rect);

    // Render text in the center of the button (you may need to adjust based on your font size)
    // Assuming you have a function to render text on renderer
    // renderText(renderer, buttonText, rect.x + rect.w / 2 - textWidth / 2, rect.y + rect.h / 2 - textHeight / 2);

    // You need to implement the renderText function or use SDL_ttf for text rendering
}
