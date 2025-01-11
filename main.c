#include "include/raylib.h"

#include <stdio.h>
#include <stdlib.h>

// structs
typedef struct Player
{
    Vector2 position;
    Vector2 velocity;
    Texture2D sprite; 
} Player;

typedef struct Players 
{
    Player* node;
    Players* next;
} Players;


// Global Variables
Players *enemyListHead;
Player *player;

char string[100];

void setPerson(Player* target, Vector2 initPos, Vector2 initVel, char* sprite){
    TraceLog(LOG_DEBUG, "Loading person %s", sprite);
    target->position = initPos;
    target->velocity = initVel;
    target->sprite = LoadTexture(sprite);

}

void drawPerson(Player* target){
    DrawTexture(target->sprite, target->position.x, target->position.y, WHITE);
}


Players* initEnemyPoll(int maxEnemys)
{
    Players *newNode, *headNode = NULL;
    for (int i = 0 ; i < maxEnemys; i++){
        newNode = malloc(sizeof(Players));
        newNode->node = malloc(sizeof(Player));
        setPerson(newNode->node, (Vector2){50.0f*i, 50.0f*i}, (Vector2){0.0f,0.0f}, "resources/enemy.png");
        
        newNode->next = headNode;
        headNode = newNode;
        
    }
    return headNode;
}

void drawPlayers(Players* enemyList, Player* playerNode ){
    Players *currentEnemy;
    BeginDrawing();
            ClearBackground(WHITE);
            for(currentEnemy = enemyList; currentEnemy != NULL ; currentEnemy = currentEnemy->next)
            {
                drawPerson(currentEnemy->node);
            }

            drawPerson(&playerNode);

        EndDrawing();
}

int main(){
    // Config Screen
    TraceLog(LOG_DEBUG, "- Init Raylib");
    InitWindow(800, 600, "Titulo"); 
    SetTargetFPS(30);

    // Load Enemy list
    TraceLog(LOG_DEBUG, "- Loading ENEMYS");
    
    enemyListHead = initEnemyPoll(6);

    if(enemyListHead == NULL){
        TraceLog(LOG_ERROR, "Failed to load ENEMY LIST");
        return 1;
    }

    // Load Player
    TraceLog(LOG_DEBUG, "- Loading PLAYER");
    player = malloc(sizeof(Player));

    if(player == NULL){
        TraceLog(LOG_ERROR, "Failed to load PLAYER");
        return 1;
    }

    setPerson(player, (Vector2){20.0f, 200.0f}, (Vector2){0.0f, 0.0f}, "resources/cheese.png" );
  
    // GAME LOOP
    while (!WindowShouldClose()) {
        // INPUT HANDLE
        
        // UPDATE

        // DRAW
        drawPlayers(enemyListHead, player);    
    }

    CloseWindow();
    free(player);
     
    //FREE enemy list
    Players *temp; 
    int i = 0;

    while(enemyListHead != NULL){
        temp = enemyListHead->next;
        free(enemyListHead);
        enemyListHead = temp;
        i++;
    } 

    return 0;
}