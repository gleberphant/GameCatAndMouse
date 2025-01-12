#include "include/raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define W_WIDTH 800
#define W_HEIGHT 600
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
  
    srand(time(NULL));
    unsigned int enemyVel = 4;
    // GAME LOOP
    while (gameStage == GAME) {
        
        // INPUT HANDLE
        if(WindowShouldClose() == true)
        {
            gameStage = EXIT;
        }

        if(IsKeyDown(KEY_UP)){
            player->moviment.y -= 10;
        }
        if(IsKeyDown(KEY_DOWN)){
            player->moviment.y += 10;
        }
        if(IsKeyDown(KEY_LEFT)){
            player->moviment.x -= 10;
        }
        if(IsKeyDown(KEY_RIGHT)){
            player->moviment.x += 10;
        }
        
        if(IsKeyDown(KEY_L)){
            enemyVel ++;
        }
        if(IsKeyDown(KEY_K) && enemyVel > 1){
            enemyVel --;           
        }

        // UPDATE STATUS
        
        player->position.x += player->moviment.x; 
        player->position.y += player->moviment.y;
        
        
            // check boundaries
            if (player->position.x > W_WIDTH - player->sprite.width 
            || player->position.x < 1 )
            {
                player->position.x -= player->moviment.x;
            }

            if (player->position.y > W_HEIGHT - player->sprite.height
            || player->position.y < 1 )
            {
                player->position.y -= player->moviment.y;
            }
        
        player->moviment = (Vector2){0,0};

        for(Players* currentEnemy = enemyListHead; currentEnemy != NULL; currentEnemy = currentEnemy->next )
        {
            // Comportamento de perseguição
            if (player->position.x > currentEnemy->node->position.x){
                currentEnemy->node->moviment.x += (rand() % enemyVel)   ;
            }else{
                currentEnemy->node->moviment.x -= (rand() % enemyVel)  ;
            }

            if (player->position.y > currentEnemy->node->position.y){
                currentEnemy->node->moviment.y += (rand() % enemyVel)  ;
            }else{
                currentEnemy->node->moviment.y -= (rand() % enemyVel)  ;
            }

            // movimenta
            currentEnemy->node->position.x += currentEnemy->node->moviment.x;            
            currentEnemy->node->position.y += currentEnemy->node->moviment.y;


            // check boundaries
            if (currentEnemy->node->position.x > W_WIDTH - currentEnemy->node->sprite.width 
            || currentEnemy->node->position.x < 1 )
            {
                currentEnemy->node->position.x -= currentEnemy->node->moviment.x;
            }

            if (currentEnemy->node->position.y > W_HEIGHT - currentEnemy->node->sprite.height
            || currentEnemy->node->position.y < 1)
            {
                currentEnemy->node->position.y -= currentEnemy->node->moviment.y;
            }

            currentEnemy->node->moviment.x=0;
            currentEnemy->node->moviment.y=0;
            
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