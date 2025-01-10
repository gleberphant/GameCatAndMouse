#include "raylib.h"

#include <stdio.h>



int main(){

    int posX=0, posY=300;
    InitWindow(800, 600, "Titulo");

    float frametime=0;
    char string[100];

    Texture2D sprite = LoadTexture("resources/cheese.png");

    printf(">>> %f", frametime);
    SetTargetFPS(30);

    while (!WindowShouldClose()) {

        frametime = GetFrameTime();

        posX += 100 * frametime;


        sprintf(string, "FRAME TIME> %f e FPS > %d",frametime, GetFPS() );
        


        if (posX > 800 ){ posX = 0;}

        BeginDrawing();
        
            ClearBackground(WHITE);

            DrawTexture(sprite, posX, posY, WHITE);
            DrawText(string, 100,100,30, BLACK);
            //DrawCircle(400, 300, 4.0f, RED);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}