#include "include/raylib.h"

#include <stdio.h>
#include <stdlib.h>


char string[100];

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    Texture2D sprite; 
} Person;

void setPerson(Person* target, Vector2 initPos, Vector2 initVel, char* sprite){
    TraceLog(LOG_DEBUG, "Loading person %s", sprite);
    target->position = initPos;
    target->velocity = initVel;
    target->sprite = LoadTexture(sprite);

}

void drawPerson(Person* target){
    DrawTexture(target->sprite, target->position.x, target->position.y, WHITE);
}

void drawFps(){
            sprintf(string, "FPS > %d", GetFPS() );
            DrawText(string, 100,100,30, BLACK);
}

int main(){
    // Config Screen
    TraceLog(LOG_DEBUG, "- Init Raylib");
    InitWindow(800, 600, "Titulo"); 

    SetTargetFPS(30);


    // Load Objects
    int numPersons
    TraceLog(LOG_DEBUG, "- Loading PERSONS");
    
    Person *player = malloc(sizeof(Person));

    if(player == NULL){
        TraceLog(LOG_ERROR, "Failed to load person");
        return 1;
    }

    setPerson(player, (Vector2){20.0f, 200.0f}, (Vector2){0.0f, 0.0f}, "resources/cheese.png" );
  

    while (!WindowShouldClose()) {
        // INPUT
        
        // UPDATE

        // DRAW
        BeginDrawing();
        
            ClearBackground(WHITE);

            drawPerson(player);

        EndDrawing();
    }

    CloseWindow();
    free(player);

    return 0;
}