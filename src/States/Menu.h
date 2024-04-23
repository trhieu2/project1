#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <string>
#include <vector>

class Menu {
public:
    Menu(SDL_Renderer* renderer, int screenWidth, int screenHeight);

    void Render();
    void HandleEvents(SDL_Event& event);
    int GetSelectedOption() const;

private:
    SDL_Renderer* renderer_;
    int screenWidth_;
    int screenHeight_;

    std::vector<std::string> options_;
    int selectedOption_;
};

#endif // MENU_H
