#include "Menu.h"
#include "TextManager.h" // You may need to include this header for text rendering

Menu::Menu(SDL_Renderer* renderer, int screenWidth, int screenHeight)
    : renderer_(renderer), screenWidth_(screenWidth), screenHeight_(screenHeight),
      selectedOption_(0) {
    options_.push_back("Play");
    options_.push_back("Quit");
}

void Menu::Render() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    // Render menu options
    int yPos = screenHeight_ / 2 - (options_.size() * 20);
    for (size_t i = 0; i < options_.size(); ++i) {
        TextManager::RenderTextt(renderer_, options_[i], screenWidth_ / 2 - 50, yPos);
        yPos += 40;
    }

    SDL_RenderPresent(renderer_);
}

void Menu::HandleEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                selectedOption_ = (selectedOption_ == 0) ? options_.size() - 1 : selectedOption_ - 1;
                break;
            case SDLK_DOWN:
                selectedOption_ = (selectedOption_ == options_.size() - 1) ? 0 : selectedOption_ + 1;
                break;
            case SDLK_RETURN:
                // Handle selection
                if (selectedOption_ == 0) {
                    // Play option selected
                    // Implement play functionality here
                } else if (selectedOption_ == 1) {
                    // Quit option selected
                    SDL_Event quitEvent;
                    quitEvent.type = SDL_QUIT;
                    SDL_PushEvent(&quitEvent);
                }
                break;
            default:
                break;
        }
    }
}

int Menu::GetSelectedOption() const {
    return selectedOption_;
}
