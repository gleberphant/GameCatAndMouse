#ifndef MAPS_H_

#define MAPS_H_


#include "raylib.h"

#include "actor.h"
#include "itens.h"
#include "main.h"

#define BORDER_SIZE 16
#define TILE_SIZE 64
#define NUM_TILES_WIDTH (int)ceilf(SCREEN_WIDTH/TILE_SIZE)
#define NUM_TILES_HEIGHT (int)ceilf(SCREEN_HEIGHT/TILE_SIZE)


typedef struct MapTile{
    Texture2D* spritesheet;
    Rectangle arena;
    int** grid;
} MapTile;


bool checkMapCollision(Actor* target);
void drawMap(Actor* target );
void loadMap(const char* pathfile);

#endif