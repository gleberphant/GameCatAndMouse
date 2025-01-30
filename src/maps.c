#include "maps.h"

#include <stdio.h>
#define ATLAS_NUM_COL 8
#define ATLAS_NUM_ROW 4
int playerX = 0;
int playerY = 0;

Texture2D atlas;


float offSetX = 0, offSetY = 0;
Rectangle tileRec = {0, 0, TILE_SIZE, TILE_SIZE};
int atlasNumCol = ATLAS_NUM_COL, atlasNumRow = ATLAS_NUM_ROW;
const int maxTilesHeight = (int)ceilf((float)SCREEN_HEIGHT/TILE_SIZE)+1, maxTilesWidth = (int)ceilf((float)SCREEN_WIDTH/TILE_SIZE)+1;
int tileIndex = 0, indexX=0, indexY=0;

// carrega mapa com valores default
int map[13][17]= {
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10},
    {8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10},
    {8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10},
    {8,9,9,9,9,0,2,9,9,9,9,9,9,9,9,10},
    {8,9,9,9,9,16,18,9,9,9,9,9,9,9,9,10},
    {8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10},
    {8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10},
    {8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10},
    {8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10},
    {8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10},
    {16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,18}
};


// carrega mapa
/**
 * @brief Carrega o mapa do jogo.
 * 
 * Esta função carrega o tileset e o mapa do jogo a partir de um arquivo.
 * 
 * @param pathfile O caminho para o arquivo do tileset.
 */
void loadMap(const char* pathfile) {
    // carrega tileset
    atlas = LoadTexture(pathfile);

    // carregar tile map do arquivo
    FILE* filemap = fopen("resources/map01.data", "r");

    if (filemap == NULL) {
        TraceLog(LOG_DEBUG, "Failed to load map\n");
    }
    for (int row = 0; row < NUM_TILES_HEIGHT; row++){
        for (int col = 0; col < NUM_TILES_WIDTH; col++) {
            fscanf(filemap, "%d,", &map[row][col]);
        }
    }

    fclose(filemap);

}

/**
 * @brief Descarrega o mapa do jogo.
 * 
 * Esta função libera os recursos alocados para o tileset.
 */
void unloadMap() {
    UnloadTexture(atlas);
}

// verifica se um objeto está dentro de um retangulo.
/**
 * @brief Verifica colisão no mapa.
 * 
 * Esta função verifica se um objeto está colidindo com um tile no mapa.
 * 
 * @param target O ator a ser verificado.
 * @return bool Retorna true se houver colisão, false caso contrário.
 */
bool checkMapCollision(Actor* target){

    playerX = (int) floorf((target->position.x-offSetX )/TILE_SIZE);
    playerY = (int) floorf((target->position.y-offSetY )/TILE_SIZE);

    if (map[playerY][playerX] != 9 && map[playerY][playerX] != 4) {
        return true;
    }


    return false;
}

/**
 * @brief Desenha o mapa do jogo.
 * 
 * Esta função renderiza o mapa do jogo na tela.
 * 
 * @param target O ator a ser desenhado no mapa.
 */
void drawMap(Actor* target ) {

    playerX = (int) floorf((target->position.x-offSetX )/TILE_SIZE);
    playerY = (int) floorf((target->position.y-offSetY )/TILE_SIZE);

    // desenha tiles
    for (int i = 0; i < maxTilesWidth; i++) {
        for (int j = 0; j < maxTilesHeight; j++) {

            tileIndex = map[j][i];

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

        }
    }

    // modo debug
    if (debugMode) {
        BeginBlendMode(BLEND_MULTIPLIED);
        DrawCircle(
            (playerX * TILE_SIZE)+offSetX+32 ,
            (playerY * TILE_SIZE)+offSetY+32 ,
            TILE_SIZE/2,
            ColorAlpha(RED, 0.5f)
        );
        EndBlendMode();
        // desenha linhas
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