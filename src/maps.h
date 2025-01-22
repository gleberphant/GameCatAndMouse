#ifndef MAPS_H_

#define MAPS_H_


#include "raylib.h"

#include "actor.h"
#include "itens.h"
#include "main.h"

#define BORDER_SIZE 16
#define TILE_SIZE 64
#define MAP_TILE_WIDTH (int)ceilf(SCREEN_WIDTH/TILE_SIZE)
#define MAP_TILE_HEIGHT (int)ceilf(SCREEN_HEIGHT/TILE_SIZE)


// TODO : ESTRUTURA DE MAPA 
typedef struct MapTile{
    Texture2D* spritesheet;
    //int grid[MAP_TILE_WIDTH][MAP_TILE_WIDTH];
} MapTile;


bool isInside(Actor* target, Rectangle *arena);
void drawMap();

#endif