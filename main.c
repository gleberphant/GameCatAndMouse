#include "include/raylib.h"

#include <stdio.h>
#include <stdlib.h>

// structs
typedef struct Player Player;

struct Player{
    Vector2 position;
    Vector2 moviment;
    Texture2D sprite;
};

typedef struct Players Players;

struct Players{
    Player* node;
    Players* next;
};

// Global Variables
Players *enemyListHead, *itemListHead;
Player *player;

typedef enum StageType { 
    INTRO, 
    MENU, 
    GAME, 
    OVER,
    EXIT 
} StageType;


StageType gameStage;

Vector2 mapEnemys[] = {
    (Vector2){2.0f, 200.0f},
    (Vector2){100.0f, 200.0f},
    (Vector2){320.0f, 100.0f},
    (Vector2){100.0f, 20.0f}
};

char string[100];

void setPerson(Player* target, Vector2 initPos, Vector2 initVel, char* sprite){
    TraceLog(LOG_DEBUG, "Loading person %s", sprite);
    target->position = initPos;
    target->moviment = initVel;
    target->sprite = LoadTexture(sprite);

}

void drawPerson(Player* target){
    DrawTexture(target->sprite, target->position.x, target->position.y, WHITE);
}


Players* initPoll( Vector2 mapPositions[], short maxNodes)
{
    Players *newListNode, *headListNode = NULL;
    
    for (int i = 0 ; i < maxNodes; i++){
        newListNode = malloc(sizeof(Players));
        newListNode->node = malloc(sizeof(Player));

        setPerson(newListNode->node, (Vector2)mapPositions[i], (Vector2){0.0f,0.0f}, "resources/cat.png");
        
        newListNode->next = headListNode;
        headListNode = newListNode;
        
    }
    return headListNode;
}


void drawPlayers(Players* enemyList, Player* playerNode ){
    Players *currentEnemy;
    BeginDrawing();
            ClearBackground(WHITE);
            for(currentEnemy = enemyList; currentEnemy != NULL ; currentEnemy = currentEnemy->next)
            {
                drawPerson(currentEnemy->node);
            }

            drawPerson(playerNode);

        EndDrawing();
}

int gameLoop(){

// Load Enemy list
    TraceLog(LOG_DEBUG, "- Loading ENEMYS");
    
    enemyListHead = initPoll(mapEnemys, ( unsigned short )sizeof(mapEnemys)/sizeof(Vector2));

    // itemListHead = initPoll((Vector2* ){233, 30});

    if(enemyListHead == NULL){
        TraceLog(LOG_ERROR, "Failed to load ENEMY LIST");
        gameStage = EXIT;
        return 1;
    }

    // Load Player
    TraceLog(LOG_DEBUG, "- Loading PLAYER");
    player = malloc(sizeof(Player));

    if(player == NULL){
        TraceLog(LOG_ERROR, "Failed to load PLAYER");
        gameStage = EXIT;
        return 1;
    }

    setPerson(player, (Vector2){400.0f, 200.0f}, (Vector2){0.0f, 0.0f}, "resources/mouse.png" );
  
    // GAME LOOP
    while (gameStage == GAME) {
        if(WindowShouldClose() == true)
        {
            gameStage = EXIT;
        }
        // INPUT HANDLE
        player->moviment = (Vector2){0,0};

        if(IsKeyPressed(KEY_UP)){
            player->moviment.y -= 1;
        }
        if(IsKeyPressed(KEY_DOWN)){
            player->moviment.y += 1;
        }
        if(IsKeyPressed(KEY_LEFT)){
            player->moviment.x -= 1;
        }
        if(IsKeyPressed(KEY_RIGHT)){
            player->moviment.x += 1;
        }
        // UPDATE

        player->position.x += 10* player->moviment.x; 
        player->position.y += 10* player->moviment.y;

        for(Players* currentEnemy = enemyListHead; currentEnemy != NULL; currentEnemy = currentEnemy->next )
        {
            if (player->position.x > currentEnemy->node->position.x){
                currentEnemy->node->moviment.x = 1;
            }else{
                currentEnemy->node->moviment.x = -1;
            }

            if (player->position.y > currentEnemy->node->position.y){
                currentEnemy->node->moviment.y = 1;
            }else{
                currentEnemy->node->moviment.y = -1;
            }

            currentEnemy->node->position.x += 2 * currentEnemy->node->moviment.x;
            currentEnemy->node->position.y += 2 * currentEnemy->node->moviment.y;

            currentEnemy->node->moviment = (Vector2){0,0};            
        }
        // DRAW
        drawPlayers(enemyListHead, player);    
    }

    
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
}

int main(){
    // Config Screen
    TraceLog(LOG_DEBUG, "- Init Raylib");
    InitWindow(800, 600, "CatAndMouse"); 
    SetTargetFPS(30);
    gameStage = INTRO;
    
    while(gameStage != EXIT){

        if (gameStage == INTRO){
            Texture2D bgTexture = LoadTexture("resources/intro_bg.png");
            while (gameStage == INTRO){
         
                if(IsKeyPressed(KEY_ENTER)){
                    gameStage == GAME;
                }

                BeginDrawing();
                    DrawTexture(bgTexture,0,0, WHITE);
                EndDrawing();   
            }
        }
        if (gameStage == GAME){
            gameLoop();
        }
    }
    

     

    CloseWindow();
    return 0;
}