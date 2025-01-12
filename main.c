#include "include/raylib.h"
#include "include/raymath.h"


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
    Vector2 size;
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
    target->size.x = target->sprite.width;
    target->size.y = target->sprite.height;
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

bool isInside(Vector2 position, Vector2 size, Rectangle *arena){
    Rectangle  targetBox ={
        position.x,
        position.y,
        size.x,
        size.y
    };

    return CheckCollisionRecs(targetBox, *arena);
    // if ( targetBox.x < arena->x || targetBox.x > arena->width-targetBox.width){
    //     return false;
    // }
    // if( targetBox.y < arena->y || targetBox.y > arena->height-targetBox.height){
    //     return false;
    // }

    // return true;
    

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

    
    // load map

    
    Rectangle arena = {
        player->size.x,
        player->size.y, 
        W_WIDTH - player->size.x*2, 
        W_HEIGHT - player->size.y*2
    };
    
    // SET VARIABLES
    Vector2 newPos;
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
        newPos = Vector2Add(player->position, player->moviment);
        
        // check boundaries
        if(isInside(newPos, player->size, &arena)){
            player->position = newPos;
        }
        player->moviment = Vector2Zero();

        for(Players* currentEnemy = enemyListHead; currentEnemy != NULL; currentEnemy = currentEnemy->next )
        {
            Vector2 oldPos = currentEnemy->node->position;

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
            currentEnemy->node->position = Vector2Add(currentEnemy->node->position, currentEnemy->node->moviment);

            // check boundaries
            if (!isInside(currentEnemy->node->position, currentEnemy->node->size,&arena))
            {
                currentEnemy->node->position = oldPos;
            }
            
            currentEnemy->node->moviment = Vector2Zero();
            
        }
        // DRAW
        DrawRectangleRec(arena,GRAY);
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