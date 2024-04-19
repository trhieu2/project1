#include "TextManager.h"

TextManager::TextManager()
{
    text_color.r = 255;
    text_color.g = 255;
    text_color.b = 255;
    texture = NULL;
}

TextManager::~TextManager()
{

}

bool TextManager::LoadFromRenderText(TTF_Font* font, SDL_Renderer* screen)
{
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val.c_str(), text_color);
    if(text_surface)
    {
        texture = SDL_CreateTextureFromSurface(screen, text_surface);
        width = text_surface->w;
        height = text_surface->h;

        SDL_FreeSurface(text_surface);
    }
    return texture != NULL;
}

void TextManager::Free()
{
    if(texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void TextManager::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color.r = red;
    text_color.g = green;
    text_color.b = blue;
}

void TextManager::SetColor(int type)
{
    if(type == RED_TEXT)
    {
        SDL_Color color = {255, 0, 0};
        text_color = color;
    }
    else if(type == WHITE_TEXT)
    {
        SDL_Color color = {255, 255, 255};
        text_color = color;
    }
    else if(type == BLACK_TEXT)
    {
        SDL_Color color = {0, 0, 0};
        text_color = color;
    }
}

void TextManager::RenderTextt(SDL_Renderer* screen, int xp, int yp, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect renderquad = {xp, yp, width, height};
    if(clip != NULL)
    {
        renderquad.w = clip->w;
        renderquad.h = clip->h;
    }
    SDL_RenderCopyEx(screen, texture, clip, &renderquad, angle, center, flip);
}

