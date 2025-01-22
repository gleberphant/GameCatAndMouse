#include "maps.h"

int playerX = 0;
int playerY = 0;



int offSet = 15;
// verifica se um objeto está dentro de um retangulo.
bool isInside(Actor* target, Rectangle *arena){

    arena->height = MAP_TILE_HEIGHT*TILE_SIZE;
    arena->width = MAP_TILE_WIDTH*TILE_SIZE;

    playerX = (int) floorf((target->position.x-offSet )/TILE_SIZE);
    playerY = (int) floorf((target->position.y-offSet )/TILE_SIZE);

    if ((playerX) > MAP_TILE_WIDTH -1 ||
        (playerX) < 0 ||
        (playerY) > MAP_TILE_HEIGHT -1||
        (playerY) < 0 ) {
        return false;
    }

    return true;
}



void drawMap() {

    for (int i = 0; i < MAP_TILE_WIDTH; i++) {
        for (int j = 0; j < MAP_TILE_HEIGHT; j++) {
            if ( playerX == i && playerY == j) {
                DrawRectangle(
                    (i*TILE_SIZE)+offSet ,
                    (j*TILE_SIZE)+offSet ,
                    TILE_SIZE,
                    TILE_SIZE,
                    LIGHTGRAY
                );
            }
            else {
                DrawRectangleLines(
                (i*TILE_SIZE)+offSet ,
                (j*TILE_SIZE)+offSet ,
                TILE_SIZE,
                TILE_SIZE,
                DARKBLUE
                );
            }
        }
    }
}