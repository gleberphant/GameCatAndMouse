#include "scene_game.h"
#include "scene.h"
#include "main.h"
#include "actor.h"
#include "itens.h"
#include "maps.h"  
#include "lists.h"
#include "raymath.h"    
#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>


Actor *player, *currentActor = NULL;
Item *currentItem = NULL;

ActorNode *enemyListHead = NULL, *playerListHead = NULL;
ItemNode *itemListHead = NULL;

Rectangle arena;
float volumeMaster = 1.01f, angulo;
int level=0, score=0, enemyVel=0;

Texture2D *itemSpriteSheetArray, *catSpriteSheet;
Sound eatCheese, eatStrawberry, getHit;

Music bgMusic;

short enemyType = 0, enemyCount = 0;
Vector2 targetPosition;


void initSceneGame(){

    // SETAR VARIÁVEIS DO LOOP
    TraceLog(LOG_DEBUG, "== definindo GLOBAL VARIABLES");
    if (debugMode)volumeMaster = 0;
    enemyVel = 3;

    // Carregar o mapa
    TraceLog(LOG_DEBUG, "== carregando MAPAS");
    Vector2 mapEnemies[] = {
        (Vector2){128.0f, 140.0f},
        (Vector2){250.0f, 420.0f}

    };

    InitListItens initItens[] = {
        (InitListItens){.type = CHEESE, .initPos = (Vector2){100.0f, 120.0f} },
        (InitListItens){.type = CHEESE, .initPos = (Vector2){600.0f, 500.0f} },
        (InitListItens){.type = TRAP,   .initPos = (Vector2){300.0f, 300.0f} }
    };

    arena = (Rectangle){
        .x = MAP_BORDER,
        .y = MAP_BORDER,
        .width = SCREEN_WIDTH - (MAP_BORDER * 2),
        .height = SCREEN_HEIGHT - (MAP_BORDER * 2)
    };

    // CARREGAR LISTA DE INIMIGOS
    TraceLog(LOG_DEBUG, "== carregando ENEMY LIST");

    catSpriteSheet = getSpriteSheet("resources/catA");
    enemyListHead = getActorList(
        mapEnemies,
        catSpriteSheet,
        sizeof(mapEnemies) / sizeof(Vector2)
    );

    if (enemyListHead == NULL) {
        TraceLog(LOG_ERROR, " ::: ERROR ao carregar ENEMY LIST");
        currentSceneType = EXIT;
    }

    // CARREGAR LISTA DE ITENS
    TraceLog(LOG_DEBUG, "== carregando ITENS LIST");

    const char *spritepathList[] = {
    "resources/item_cheese.png",
    "resources/item_strawberry.png",
    "resources/item_trap.png"
    };


    itemSpriteSheetArray = getItemSpriteSheetArray(spritepathList);


    itemListHead = getItemList(
        initItens,
        itemSpriteSheetArray,
        sizeof(initItens) / sizeof(InitListItens)
    );

    if (itemListHead == NULL) {
        TraceLog(LOG_ERROR, "::: ERROR ao carregar ITENS LIST");
        currentSceneType = EXIT;
    }

    // Carregar personagem do jogador
    TraceLog(LOG_DEBUG, "== carregando PLAYER");

    playerListHead = getActorList(
        (Vector2[]){PLAYER_INIT_POS},
        getSpriteSheet("resources/mouseA"),
        1
    );

    playerListHead->obj->speed = 10;

    player = playerListHead->obj;
    // Inicializar pontuação e nível
    score = 0;
    level = 1;


    // CARREGAR EFEITOS SONOROS
    TraceLog(LOG_DEBUG, "== carregando EFEITOS SONOROS");
    eatCheese = LoadSound("resources/sounds/eat_cheese.mp3");
    eatStrawberry = LoadSound("resources/sounds/eat_strawberry.mp3");
    getHit = LoadSound("resources/sounds/get_hit.mp3");
    bgMusic = LoadMusicStream("resources/sounds/game_music.mp3");    
 
    loadMap("resources/tileset.png");

    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic, volumeMaster);
    SetExitKey(KEY_NULL);

}

bool handlePlayerInput(){

    if (IsKeyPressed(KEY_ESCAPE)) {
        currentSceneType = INTRO;
        return false;
    }

    inputScene(currentScene);

    if(debugMode){
            SetMasterVolume(0.0f);

        if (IsKeyDown(KEY_L)) {
            enemyVel++;
        }

        if (IsKeyDown(KEY_K) && enemyVel > 1) {
            enemyVel--;
        }

        if (IsKeyReleased(KEY_KP_ADD)) {
            volumeMaster += 0.05f;
            SetMasterVolume(volumeMaster);
        }

        if (IsKeyReleased(KEY_KP_SUBTRACT)) {
            volumeMaster -= 0.05f;
            SetMasterVolume(volumeMaster);
        }
        if (IsKeyReleased(KEY_SPACE)) {                
            if (player->action == SPECIAL) player->action = STOP;
            else player->action = SPECIAL;
            player->spriteA2[SPECIAL]->currentFrame = 0;
        }
    }

    // CONTROL PLAYER
    
    if(player->action == SPECIAL) return true;

    player->action = STOP;
    Vector2 moveTarget = Vector2Zero();

    // controle por teclado
    if (IsKeyDown(KEY_UP)) {
        player->action = MOVE;
        moveTarget.y +=  -1.0f;    
    }  

    if (IsKeyDown(KEY_RIGHT)) {
        player->action = MOVE;
        moveTarget.x +=  1.0f;    
    }  
    
    if (IsKeyDown(KEY_DOWN)) {
        player->action = MOVE;
        moveTarget.y +=  1.0f;    
    }  
    
    if (IsKeyDown(KEY_LEFT)) {
        player->action = MOVE;
        moveTarget.x +=  -1.0f;    
    }


    // controle por mouse
    if (
    IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
    Vector2Distance(player->position, GetMousePosition()) > player->collisionBox.width
    ) {

        player->action = MOVE;
        moveTarget = Vector2Subtract(GetMousePosition(), player->position);   
    }

    if (player->action == MOVE){
        player->direction = Vector2LineAngle(
                                Vector2Zero(),
                                moveTarget
                            ) * -1.0f;
        
    }

return true;
}


void updateMap() {
    // Verifica o nível atual
    if (score > level * 20) {
        enemyVel += 1;
        level++;


        Vector2 newEnemyPos[] = {
        (Vector2){250.0f, 420.0f}
        };

        ActorNode* newCatNode = getActorList(
        newEnemyPos,
        catSpriteSheet,
        sizeof(newEnemyPos) / sizeof(Vector2)
        );

        newCatNode->next = enemyListHead;
        enemyListHead = newCatNode;

    }

}

void updatePlayer(){

// Condições de vitória ou derrota.
        if (player->life < 1) {
            currentSceneType = OVER;
            return;
        }

        // ações do jogador
        switch (player->action) {
            case STOP:
                actionStop(player);
                break;

            case MOVE:
                actionMove(player, &arena);
                break;

            case SPECIAL:
                actionSpecial(player, player);
                break;

            case END:
                TraceLog(LOG_DEBUG, "fim da linha");
                break;

            default:
                TraceLog(LOG_DEBUG, "item desconhecido");
                break;
        }

}

void updateItens(){

        // ------------------------------------------------------------
        // "ações" dos itens
        for (ItemNode *currentNode = itemListHead, *prev = NULL; currentNode != NULL;
             prev = currentNode, currentNode = currentNode->next) {
            currentItem = currentNode->obj;


            // checa colisão com item
            if (CheckCollisionRecs(player->collisionBox, currentItem->collisionBox)) {
                switch (currentItem->type) {
                    case CHEESE:
                        PlaySound(eatCheese);

                        currentItem->life = -1;
                        score += 5;
                        break;

                    case STRAWBERRY:
                        PlaySound(eatStrawberry);

                        currentItem->life = -1;
                        player->life += 10;
                        break;

                    case TRAP:

                        currentItem->life = -1;
                        player->life -= 10;
                        player->action = SPECIAL;
                        break;
                    default:
                        TraceLog(LOG_DEBUG, "item desconhecido");
                        break;
                }
            }
       // criar item novo e remover anterior
            currentItem->life--;

            if (currentItem->life < 1) {
                // REMOVER O NÓ
                if (prev == NULL) {
                    prev = currentNode->next;
                    itemListHead = prev;
                } else prev->next = currentNode->next;
                TraceLog(LOG_DEBUG, "== liberando ITEM %p", currentNode);
                free(currentNode);

                currentNode = prev;
                if (currentNode == NULL) break;

                // CRIA UM NOVO ITEM ALEATÓRIO
                ItemNode *newNode = malloc(sizeof(ItemNode));;
                newNode->obj = malloc(sizeof(Item));


                newNode->obj = getItem(
                    (Vector2){
                        (float) GetRandomValue(64, SCREEN_WIDTH - 64), (float) (float) GetRandomValue(64, SCREEN_HEIGHT - 64)
                    },
                    itemSpriteSheetArray,
                    GetRandomValue(CHEESE, TRAP)
                );

                currentItem = newNode->obj;

                if (CheckCollisionRecs(player->collisionBox, currentItem->collisionBox)) {
                    currentItem->collisionBox = getCollisionBox(
                        (Vector2){(float) GetRandomValue(64, SCREEN_WIDTH - 64), (float) (float) GetRandomValue(64, SCREEN_HEIGHT - 64)}
                    );

                }



                newNode->next = currentNode->next;
                currentNode->next = newNode;
            }
        }

}

void updateEnemies(){
        // ------------------------------------------------------------
        enemyCount = 0;
        // "ações" dos INIMIGOS
        for (ActorNode *currentNode = enemyListHead; currentNode != NULL; currentNode = currentNode->next) {
            // verifica se está vivo
            if (currentNode->obj->life < 1) continue;
            else enemyCount++;
            
            // zera variáveis
            currentActor = currentNode->obj;

            currentActor->action = MOVE;
            currentActor->speed =  GetRandomValue(0 ,  enemyVel);// velocidade variavel
            currentActor->count++;
            targetPosition = player->position;

            if (currentActor->count > 90){
                currentActor->behavior = GetRandomValue(0, 2);
                currentActor->count = 0 ;
            }

            switch (currentActor->behavior){
            case 1:
                // protege o queijo quando jogador não estiver muito próximo.
                if ( Vector2Distance(currentActor->position, player->position) > 128 && player->action != SPECIAL ){
                    for (ItemNode *currentItem = itemListHead; currentItem != NULL; currentItem = currentItem->next) {
                        if (currentItem->obj->type == CHEESE ) {
                            targetPosition = currentItem->obj->position;
                            //currentActor->speed =   enemyVel;
                            currentActor->action =  Vector2Distance(currentActor->position, targetPosition) < 64 ? STOP : MOVE;
                            break;
                        }
                    }
                }
                break;

            case 2:
                // protege o morando quando jogador não estiver muito próximo.
                if ( Vector2Distance(currentActor->position, player->position) > 128 && player->action != SPECIAL ){
                    for (ItemNode *currentItem = itemListHead; currentItem != NULL; currentItem = currentItem->next) {
                        if ( currentItem->obj->type == STRAWBERRY) {
                            targetPosition = currentItem->obj->position;
                            //currentActor->speed =   enemyVel;
                            currentActor->action =  Vector2Distance(currentActor->position, targetPosition) < 64 ? STOP : MOVE;
                            break;
                        }
                    }
                }
                break;

            default:// comportamento de dormir se jogador estiver longe
                if (Vector2Distance(currentActor->position, player->position) > 400 && player->action != SPECIAL ) {
                        currentActor->action = STOP;
                }
                break;


            };


            // move action
            switch(currentActor->action){
                case MOVE:
                    currentActor->direction = 
                    Vector2Distance(currentActor->position, targetPosition) > 64 ? Vector2LineAngle(
                                currentActor->position,
                                targetPosition) * -1: currentActor->direction;
                                        
                    actionMove(currentActor,  &arena);

                    break;
                
                default:
                    actionStop(currentActor);
                    break;
            };

            if (CheckCollisionRecs(player->collisionBox, currentActor->collisionBox)) {
                PlaySound(getHit);
                currentActor->collision = true;
                currentActor->pointOfCollision = GetCollisionRec(player->collisionBox, currentActor->collisionBox);
                
                if (!debugMode) {
                    player->life--;
                }
            }
        }
}


void updateSceneGame(){

    updateMap();
    updatePlayer();
    updateEnemies();
    updateItens();

}

void drawHud() {
    DrawRectangle(20, 20, 380, 50, ColorAlpha(SKYBLUE, 0.5f));
    DrawRectangle(150,20,player->life*2,20,LIME);
    DrawRectangleLines(150,20,200,20,BLACK);
    DrawText(TextFormat("VIDA : %d", player->life), 20, 20, 20, BLACK);
    DrawText(TextFormat("PONTOS: %d", score), 20, 40, 20, BLACK);
    DrawText(TextFormat("NÍVEL: %d", level), 750, 20, 50, BLACK);
}

void drawDebug() {
    BeginBlendMode(BLEND_ALPHA);
    DrawRectangle(15, 420, 270, 160, ColorAlpha(SKYBLUE, 0.5f));


    DrawText( TextFormat("PlayerSize: %.2f x %.2f", player->collisionBox.width, player->collisionBox.height), 20, 440, 18, BLACK);
    DrawText( TextFormat("Mouse: %d %d", GetMouseX(), GetMouseY()), 20, 460, 18, BLACK);
    DrawText( TextFormat("GriMap Pos: i %d j %d", (int) floorf(player->position.x/TILE_SIZE), (int) floorf(player->position.y/TILE_SIZE)), 20, 480, 18,BLACK);
    DrawText( TextFormat("Player Pos: x %d y %d", (int) player->position.x, (int) player->position.y ), 20, 500, 18, BLACK);
    DrawText( TextFormat("SpriteA->repeat: %d", player->spriteA2[player->action]->repeat), 20, 520, 18, BLACK);
    DrawText( TextFormat("Player->Action: %d", player->action), 20, 540, 18, BLACK);
    DrawText( TextFormat("Player->direction: %f", player->direction), 20, 560, 18, BLACK);
    DrawText(TextFormat("FPS: %d", GetFPS() ), 500, 570, 20, BLACK);
    EndBlendMode();

}

void drawSceneGame(){

    BeginDrawing();

        //limpa tela
        ClearBackground(BLACK);

        //desenha mapa
        drawMap(player);

        //desenha itens
        drawItemList(itemListHead);

        //desenha inimigos
        drawActorList(enemyListHead);

        //desenha player
        drawActorList(playerListHead);

        //desenha HUD
        drawHud();

        // debug infor
        if (debugMode) {
            drawDebug();
        }

         EndDrawing();


    }

void saveGame(){

    // carregar pontuação salva
    FILE* file = fopen("resources/score.data", "rb");
    int top_5[5] ={0,0,0,0,0};

    if (file != NULL ) {
        fread(top_5, sizeof(int), 5, file);
    }

    fclose(file);

    // verifica os recordes
    for (int i = 0; i < 5; i++) {
        // o score é um novo record?
        if (score > top_5[i]) {
            // reordena os recordes anteriores
            for (int j = 4; j > i; j--) {
                top_5[j] = top_5[j-1];
            }
            //insere o novo recorde
            top_5[i] = score;

            break;
        }
    }
     // salva o novo top 5
    file = fopen("resources/score.data","wb");
    if (file == NULL) {
        TraceLog(LOG_ERROR, "Erro ao abrir arquivo");
        return;
    }

    fwrite(top_5, sizeof(top_5), 1, file);

    fclose(file);
}

void closeSceneGame(){

    //LIBERAR MEMORIA PLAYER
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA PLAYER LIST");
    unloadActorList(playerListHead);


    //LIBERAR MEMORIA ENEMY LIST
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA ENEMY LIST");
    unloadActorList(enemyListHead);


    //LIBERAR MEMORIA ITEM LIST
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA ITEM LIST");
    for (ItemNode *temp = itemListHead, *prev = NULL; temp != NULL; prev = temp, temp = temp->next) {
        free(prev);
    }
    free(itemSpriteSheetArray);

    //LIBERAR MEMORIA MAPA
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA MAPA");

    //LIBERAR MEMORIA EFEITOS SONOROS
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA EFEITOS SONOROS");
    UnloadSound(eatCheese);
    UnloadSound(getHit);
    UnloadSound(eatStrawberry);

    //LIBERAR MEMORIA MUSICA
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA MUSICA");
    StopMusicStream(bgMusic);
    UnloadMusicStream(bgMusic);

    volumeMaster = 0.5f;

    return;

}

void loopSceneGame(){
    while (currentSceneType == GAME) {
        if (!handlePlayerInput()) {
            currentSceneType = INTRO;
            break;
        }

        updateSceneGame();

        drawSceneGame();

        UpdateMusicStream(bgMusic);
    }


}
// cena loop principal
void runSceneGame() {

    initSceneGame();
    
    loopSceneGame();

    saveGame();

    closeSceneGame();

}
//----------------------------------------------------------
// FIM DO ARQUIVO
