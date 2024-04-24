#ifndef GAMEFUNCTION_H
#define GAMEFUNCTION_H

#include <windows.h>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

//screen
const int FPS = 25;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 0Xff;

#define TILE_SIZE 64

#define MAX_X 400
#define MAX_Y 10

#define MONEY 4

typedef struct Input
{
    int left;
    int right;
    int up;
    int down;
    int jump;
};

typedef struct Map
{

    int start_x;
    int start_y;

    int max_x;
    int max_y;

    int tile[MAX_Y][MAX_X];
    char* filename;
};

namespace SDLCommonFunc
{
    bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
}

class GameFunction
{
    public:
        GameFunction();
        virtual ~GameFunction();

    protected:

    private:
};

#endif // GAMEFUNCTION_H
