#include "GameObject.h"
#include "TextureManager.h"
GameObject::GameObject(const char* texturesheet, int x, int y)
{
	objTexture = TextureManager::LoadTexture(texturesheet);

	xpos = x;
	ypos = y;
}
void GameObject::Update()
{
	xpos++;
	ypos++;


	srcRect.h = 564;
	srcRect.w = 564;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w / 10;
	destRect.h = srcRect.h / 10;
}

void GameObject::Render()
{
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}