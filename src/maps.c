#include "maps.h"
#define ATLAS_NUM_COL 8
#define ATLAS_NUM_ROW 4
int playerX = 0;
int playerY = 0;

Texture2D atlas;

float offSetX = 16, offSetY = 12;
Rectangle tileRec = {0, 0, TILE_SIZE, TILE_SIZE};
int atlasNumCol = ATLAS_NUM_COL, atlasNumRow = ATLAS_NUM_ROW;
int tileIndex = 0, indexX=0, indexY=0;

// carrega mapa
void loadMap(const char* pathfile) {
    atlas = LoadTexture(pathfile);
}


// verifica se um objeto está dentro de um retangulo.
bool isInside(Actor* target, Rectangle *arena){

    arena->height= (float) NUM_TILES_HEIGHT * TILE_SIZE;
    arena->width = (float) NUM_TILES_WIDTH  * TILE_SIZE;

    playerX = (int) floorf((target->position.x-offSetX )/TILE_SIZE);
    playerY = (int) floorf((target->position.y-offSetY )/TILE_SIZE);

    if ((playerX) > NUM_TILES_WIDTH - 1 ||
        (playerX) < 0 ||
        (playerY) > NUM_TILES_HEIGHT - 1||
        (playerY) < 0 ) {
        return false;
    }

    return true;
}


void drawMap(Actor* target ) {

    playerX = (int) floorf((target->position.x-offSetX )/TILE_SIZE);
    playerY = (int) floorf((target->position.y-offSetY )/TILE_SIZE);

    // DRAW TILES
    for (int i = 0; i < NUM_TILES_WIDTH; i++) {
        for (int j = 0; j < NUM_TILES_HEIGHT; j++) {

            tileIndex = 9;


            indexY = (int)floorf( tileIndex/atlasNumCol );

            indexX = (int)floorf( tileIndex - (indexY  * atlasNumCol) );

            tileRec.x = indexX * TILE_SIZE;
            tileRec.y = indexY * TILE_SIZE;


            DrawTexturePro(
                atlas,
                tileRec,
                (Rectangle){ ( i * TILE_SIZE ) + offSetX, ( j * TILE_SIZE ) + offSetY , TILE_SIZE, TILE_SIZE},
                (Vector2){ 0.0f, 0.0f },
                0.0f,
                WHITE
            );

            // DrawRectangleLines(
            //      (i*TILE_SIZE) + offSetX ,
            //      (j*TILE_SIZE) + offSetY ,
            //      TILE_SIZE,
            //      TILE_SIZE,
            //      BROWN
            //  );
        }
    }

    //debug mode
    if (debugMode) {
        BeginBlendMode(BLEND_MULTIPLIED);
        DrawCircle(
            (playerX * TILE_SIZE)+offSetX+32 ,
            (playerY * TILE_SIZE)+offSetY+32 ,
            TILE_SIZE/2,
            ColorAlpha(LIGHTGRAY, 0.5f)
        );
        EndBlendMode();
        //desenha linhas
        // linhas do eixo x
        for (int col = 0; col < SCREEN_WIDTH; col+=TILE_SIZE) {
            DrawLine(col+offSetY, 0, col+offSetY, SCREEN_HEIGHT, LIGHTGRAY);
        }
        // linhas do eixo y
        for (int row = 0; row < SCREEN_HEIGHT; row+=TILE_SIZE) {
            DrawLine(0, row+offSetX, SCREEN_WIDTH, row+offSetX, LIGHTGRAY);
        }
    }

}