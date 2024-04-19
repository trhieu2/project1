#ifndef GMEMAP_H
#define GMEMAP_H

#include "GameFunction.h"
#include "BaseObject.h"

#define MAX_TILE 20

class TileMap : public BaseObject
{
public:
    TileMap(){;}
    ~TileMap(){;}
};

class GmeMap
{
    public:
        GmeMap(){;}
        ~GmeMap(){;}

        void LoadMap(char* filename);
        void LoadTiles(SDL_Renderer* screen);
        void FillMap(SDL_Renderer* screen);
        Map GetMap() const {return game_map;}
        void SetMap(Map& map_data){game_map = map_data;}
    private:
        Map game_map;
        TileMap tile_map[MAX_TILE];
};

#endif // GMEMAP_H
