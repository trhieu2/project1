#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_image.h>

class Button {
public:
    Button(SDL_Renderer* renderer, const char* imagePath, const char* hoverImagePath, int x, int y, float scale) {
        // Load the images into SDL_Textures
        SDL_Surface* surface = IMG_Load(imagePath);
        SDL_Surface* hoverSurface = IMG_Load(hoverImagePath);
        if (!surface || !hoverSurface) {
            printf("IMG_Load: %s\n", IMG_GetError());
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        hoverTexture = SDL_CreateTextureFromSurface(renderer, hoverSurface);
        if (!texture || !hoverTexture) {
            printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        }

        // Set the button's attributes
        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
        rect.w *= scale;  // Scale the width
        rect.h *= scale;  // Scale the height
        rect.x = x;
        rect.y = y;

        SDL_FreeSurface(surface);
        SDL_FreeSurface(hoverSurface);
    }

    ~Button() {
        // Clean up the textures
        SDL_DestroyTexture(texture);
        SDL_DestroyTexture(hoverTexture);
    }

    void render(SDL_Renderer* renderer, bool isHovered) {
        // Render the button
        if (isHovered) {
            SDL_RenderCopy(renderer, hoverTexture, NULL, &rect);
        } else {
            SDL_RenderCopy(renderer, texture, NULL, &rect);
        }
    }

    bool isHovered(int mouseX, int mouseY) {
        // Check if the mouse is over the button
        return (mouseX > rect.x && mouseX < rect.x + rect.w &&
                mouseY > rect.y && mouseY < rect.y + rect.h);
    }

    bool isClicked(int mouseX, int mouseY) {
        // Check if the click is within the button's area
        return (mouseX > rect.x && mouseX < rect.x + rect.w &&
                mouseY > rect.y && mouseY < rect.y + rect.h);
    }

private:
    SDL_Texture* texture;
    SDL_Texture* hoverTexture;
    SDL_Rect rect;
};


#endif // BUTTON_H
