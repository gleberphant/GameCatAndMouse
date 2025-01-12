/* TODO
- CHECK COLISION
- COLISAO ITEM = MAIS PONTO
- COLISAO ENEMY = MORTE
- ELABORAR FUNÇÃO PARA ADICIONAR E REMOVER ITENS DA LISTA
*/


#include "include/raylib.h"
#include "include/raymath.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define W_WIDTH 800
#define W_HEIGHT 600
#define MAP_BORDER 10
// structs


typedef struct Player Player;
typedef struct Players Players;
typedef struct Actions Actions;
typedef enum ActionType ActionType;
typedef enum StageType StageType;

enum ActionType{
    MOVE,
    STOP
};

enum StageType { 
    INTRO, 
    MENU, 
    GAME, 
    OVER,
    EXIT 
};

struct Player{
    Vector2 position;
    Vector2 moviment;
    Vector2 size;
    Texture2D sprite;
};

struct Players{
    Player* node;
    Players* next;
};


struct Actions{
    
    Vector2 moviment;
    ActionType type;

};

// Global Variables
Players *enemyListHead, *itemListHead;
//Players *currentEnemy, *currentItem;
Player *player;

Rectangle arena;
Actions playerAction;

StageType gameStage;

Vector2 mapEnemys[] = {
    (Vector2){20.0f, 200.0f},
    (Vector2){100.0f, 200.0f},
    (Vector2){320.0f, 100.0f}
};


Vector2 mapItens[] = {
    (Vector2){20.0f, 200.0f},
    (Vector2){100.0f, 200.0f},
    (Vector2){320.0f, 100.0f}
};

char string[100];


// functions
void setPerson(Player* target, Vector2 initPos, Vector2 initVel, char* sprite){
    TraceLog(LOG_DEBUG, "Loading person %s", sprite);
    target->position = initPos;
    target->moviment = initVel;
    target->sprite = LoadTexture(sprite);
    target->size.x = target->sprite.width;
    target->size.y = target->sprite.height;
}



Players* initPoll( Vector2 mapPositions[], char* sprite, short maxNodes)
{
    Players *newListNode, *headListNode = NULL;
    
    for (int i = 0 ; i < maxNodes; i++){
        newListNode = malloc(sizeof(Players));
        newListNode->node = malloc(sizeof(Player));

        setPerson(newListNode->node, (Vector2)mapPositions[i], (Vector2){0.0f,0.0f}, sprite);
        
        newListNode->next = headListNode;
        headListNode = newListNode;
        
    }
    return headListNode;
}



void drawPerson(Player* target){
    // DrawRectangleRec(
    // (Rectangle){target->position.x-2, target->position.y-2, target->size.x+2, target->size.y+2 }, 
    // BLACK
    // );
    DrawTexture(target->sprite, target->position.x, target->position.y, WHITE);
}

void drawPlayers(Players* targetList){
    Players *currentEnemy;
    for(currentEnemy = targetList; currentEnemy != NULL ; currentEnemy = currentEnemy->next)
    {
        drawPerson(currentEnemy->node);
    }

       
}

bool isInside(Player* target, Rectangle *arena){
    Rectangle  targetBox ={
        target->position.x,
        target->position.y,
        target->size.x,
        target->size.y
    };

    // return CheckCollisionRecs(targetBox, *arena);
    if( targetBox.x < arena->x || targetBox.x > arena->width+arena->x-targetBox.width){
        return false;
    }
    if( targetBox.y < arena->y || targetBox.y > arena->height+arena->y-targetBox.height){
        return false;
    }

    return true;
    

}


int gameIntro(){

            // carregar imagem e texto de abertura
            Font fontDefault = GetFontDefault(); 
            Texture2D bgTexture = LoadTexture("resources/intro_bg.png");
            int textFontSize = 32;
            char* textIntro = "[ENTER] para jogar [ESC] para sair.";            

            Vector2 textSize = MeasureTextEx(fontDefault, textIntro, textFontSize, 2);
            Vector2 textPos = (Vector2){(W_WIDTH - textSize.x)/2, 400 };              
            
            Rectangle textBackground = { textPos.x-20, textPos.y-20, textSize.x+60, textSize.y+40 };

            // aguardar o jogador apertar ENTER ou ESC
            while (gameStage == INTRO){
         
                if(IsKeyDown(KEY_ESCAPE) || WindowShouldClose()){
                    gameStage = EXIT;
                }

                if(IsKeyDown(KEY_ENTER)){
                    gameStage = GAME;
                }

                BeginDrawing();
                    DrawTextureEx(bgTexture, (Vector2){0.0f, 0.0f}, 0.0f, 1.0f, WHITE);
                    DrawRectangleRec(textBackground, BLACK);
                    DrawText(textIntro, textPos.x, textPos.y, textFontSize, WHITE);
                EndDrawing();
            }


}


int gameLoop(){

// Load Enemy list
    TraceLog(LOG_DEBUG, "- Loading ENEMYS");
    
    enemyListHead = initPoll(
        mapEnemys,
        "resources/cat.png", 
        (unsigned short )sizeof(mapEnemys)/sizeof(Vector2)
    );

    if(enemyListHead == NULL){
        TraceLog(LOG_ERROR, "Failed to load ENEMY LIST");
        gameStage = EXIT;
        return 1;
    }

    TraceLog(LOG_DEBUG, "- Loading ITENS");
    itemListHead = initPoll(
        mapItens,
        "resources/item.png", 
        (unsigned short)sizeof(mapItens)/sizeof(Vector2)
    );

    if(itemListHead == NULL){
        TraceLog(LOG_ERROR, "Failed to load ITENS LIST");
        gameStage = EXIT;
        return 1;
    }

    // LOAD PLAYER
    TraceLog(LOG_DEBUG, "- Loading PLAYER");
    player = malloc(sizeof(Player));

    if(player == NULL){
        TraceLog(LOG_ERROR, "Failed to load PLAYER");
        gameStage = EXIT;
        return 1;
    }

    setPerson(player, (Vector2){400.0f, 200.0f}, (Vector2){0.0f, 0.0f}, "resources/mouse.png" );
    
    // LOAD MAP
    TraceLog(LOG_DEBUG, "- Loading PLAYER");
    arena.x = 0+MAP_BORDER;
    arena.y = 0+MAP_BORDER;
    arena.width = W_WIDTH-(MAP_BORDER*2); 
    arena.height = W_HEIGHT-(MAP_BORDER*2);
    
    // SET VARIABLES
    TraceLog(LOG_DEBUG, "- set VARIABLES");
    Vector2 oldPos;
    srand(time(NULL));
    unsigned short enemyVel = 4;
    
    // GAME LOOP
    while (gameStage == GAME) {
        // INPUT HANDLE
        if(WindowShouldClose() == true)
        {
            gameStage = EXIT;
        }
        
        playerAction.type = STOP;
        playerAction.moviment = Vector2Zero();
        
        if(IsKeyDown(KEY_UP)){
            playerAction.type = MOVE;
            playerAction.moviment.y -= 10;
        }
        if(IsKeyDown(KEY_DOWN)){
            playerAction.type = MOVE;
            playerAction.moviment.y += 10;
        }
        if(IsKeyDown(KEY_LEFT)){
            playerAction.type = MOVE;
            playerAction.moviment.x -= 10;
        }
        if(IsKeyDown(KEY_RIGHT)){
            playerAction.type = MOVE;
            playerAction.moviment.x += 10;
        }
        
        if(IsKeyDown(KEY_L)){
            enemyVel ++;
        }
        if(IsKeyDown(KEY_K) && enemyVel > 1){
            enemyVel --;           
        }

        // UPDATE STATUS
        
        // PLAYER
        // PLAYER ACTION
        // move action
        if(playerAction.type == MOVE){
            player->moviment = playerAction.moviment;
            oldPos = player->position;
            player->position = Vector2Add(player->position, player->moviment);
            player->moviment = Vector2Zero();
            
            // CHECK COLISION
            if(!isInside(player, &arena)){
                player->position = oldPos;
            }        
        }

        // UPDATE ENEMYS
        float distanciaX=0, distanciaY=0;
        Vector2 currentPosition, distance;
        Rectangle enemyRec, playerRec, itemRec, colisionRec;

        colisionRec = (Rectangle){0 ,0,0,0};
        playerRec = (Rectangle){ 
                player->position.x, 
                player->position.y,
                player->size.x,
                player->size.y
        };

        for(Players* itemNode = itemListHead; itemNode != NULL ; itemNode = itemNode->next){
            itemRec = (Rectangle){ 
                itemNode->node->position.x, 
                itemNode->node->position.y,
                itemNode->node->size.x,
                itemNode->node->size.y
            };

            if(CheckCollisionRecs(playerRec, itemRec)){
                
                colisionRec = GetCollisionRec(playerRec, itemRec);
                
            }
        }
        for(Players* currentEnemy = enemyListHead; currentEnemy != NULL; currentEnemy = currentEnemy->next )
        {
            // Comportamento de perseguição
            enemyRec = (Rectangle){ 
                currentEnemy->node->position.x, 
                currentEnemy->node->position.y,
                currentEnemy->node->size.x,
                currentEnemy->node->size.y
            };

            playerAction.moviment = Vector2Zero();
            playerAction.type = STOP;

            if ((enemyRec.x - player->position.x) < 0){
                playerAction.type = MOVE;
                playerAction.moviment.x += rand() % enemyVel;
            }else{
                playerAction.type = MOVE;
                playerAction.moviment.x -= (rand() % enemyVel);
            }

            if ((enemyRec.y - player->position.y) < 0){
                playerAction.type = MOVE;
                playerAction.moviment.y += (rand() % enemyVel);
                
            }else{
                playerAction.type = MOVE;
                playerAction.moviment.y -= (rand() % enemyVel);                
            }

            // ENEMY ACTION
            // move action
            currentEnemy->node->moviment = playerAction.moviment;
            oldPos = currentEnemy->node->position;
            currentEnemy->node->position = Vector2Add(currentEnemy->node->position, currentEnemy->node->moviment);
            currentEnemy->node->moviment = Vector2Zero();


            // CHECK COLISIONS
            if (!isInside(currentEnemy->node ,&arena))
            {
                currentEnemy->node->position = oldPos;
            }                   

            if(CheckCollisionRecs(playerRec, enemyRec)){
                
                colisionRec= GetCollisionRec(playerRec, enemyRec);
                
                DrawRectangleRec(playerRec,RED);    
            }

        }
        
        // DRAW
        BeginDrawing();
            // ClearBackground(WHITE);
            DrawRectangleRec(arena,GRAY);

            drawPlayers(enemyListHead);

            drawPlayers(itemListHead);
            drawPerson(player);

            DrawCircle(colisionRec.x,colisionRec.y, 10, RED);

        EndDrawing();    
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
    TraceLog(LOG_DEBUG, "-- Init Raylib");
    
    InitWindow(W_WIDTH, W_HEIGHT, "CatAndMouse - GAME"); 
    SetTargetFPS(30);

    gameStage = INTRO;
    
    while(gameStage != EXIT){

        if (gameStage == INTRO){
            gameIntro();
        }
        if (gameStage == GAME){
            gameLoop();
        }
    }   

    CloseWindow();
    return 0;
}