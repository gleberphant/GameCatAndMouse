#include "raylib.h"

#include <stdio.h>

typedef struct
{
    Vector2 posicao;
    Vector2 velocidade;
    Texture2D sprite; 
} Player ;


int main(){

    char string[100];

    InitWindow(800, 600, "Titulo");

    Player player;
    
    player.posicao.x = 0.0f;
    player.posicao.y = 0.0f;
    
    player.velocidade.x = 0.0f;
    player.velocidade.y = 0.0f;

    player.sprite = LoadTexture("resources/cheese.png");

    
    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        // INPUT
        
        // UPDATE

        // DRAW
        BeginDrawing();
        
            ClearBackground(WHITE);

            DrawTexture(player.sprite, player.posicao.x, player.posicao.y, WHITE);
            
            sprintf(string, "FPS > %d", GetFPS() );
            DrawText(string, 100,100,30, BLACK);
            DrawCircle(400, 300, 4.0f, RED);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}