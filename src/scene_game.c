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

Texture2D *itemSpriteSheet;
Sound eatCheese, eatStrawberry, getHit;

Music bgMusic;


void initSceneGame(){

    // SETAR VARIÁVEIS DO LOOP
    TraceLog(LOG_DEBUG, "== definindo GLOBAL VARIABLES");
    if (debugMode)volumeMaster = 0;
    enemyVel = 4;

    // Carregar o mapa
    TraceLog(LOG_DEBUG, "== carregando MAPAS");
    Vector2 mapEnemies[] = {
        (Vector2){20.0f, 140.0f},
        (Vector2){650.0f, 120.0f}
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
    enemyListHead = getActorList(
        mapEnemies,
        "resources/catA",
        sizeof(mapEnemies) / sizeof(Vector2)
    );

    if (enemyListHead == NULL) {
        TraceLog(LOG_ERROR, " ::: ERROR ao carregar ENEMY LIST");
        currentSceneType = EXIT;
    }

    // CARREGAR LISTA DE ITENS
    TraceLog(LOG_DEBUG, "== carregando ITENS LIST");
    itemSpriteSheet = malloc(sizeof(Texture2D) * 4);

    itemSpriteSheet[CHEESE] = LoadTexture("resources/item_cheese.png");
    itemSpriteSheet[STRAWBERRY] = LoadTexture("resources/item_strawberry.png");
    itemSpriteSheet[TRAP] = LoadTexture("resources/item_trap.png");
    itemSpriteSheet[END_ITEM] = (Texture2D){0};


    itemListHead = getItemList(
        initItens,
        itemSpriteSheet,
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
        "resources/mouseA",
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

    // EXIT SCENE
    if (WindowShouldClose()) {
        currentSceneType = EXIT;
        return false;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        currentSceneType = INTRO;
        return false;
    }

    // DEBUG OPTIONS
    if(debugMode){
        if (IsKeyReleased(KEY_F1)) {
            debugMode = !debugMode;
        }

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
    Vector2 vectorDirection = Vector2Zero();

    // controle por teclado
    if (IsKeyDown(KEY_UP)) {
        player->action = MOVE;
        vectorDirection.y +=  -1.0f;    
    }  

    if (IsKeyDown(KEY_RIGHT)) {
        player->action = MOVE;
        vectorDirection.x +=  1.0f;    
    }  
    
    if (IsKeyDown(KEY_DOWN)) {
        player->action = MOVE;
        vectorDirection.y +=  1.0f;    
    }  
    
    if (IsKeyDown(KEY_LEFT)) {
        player->action = MOVE;
        vectorDirection.x +=  -1.0f;    
    }


    // controle por mouse
    if (
        IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
        Vector2Distance(player->position, GetMousePosition()) > player->collisionBox.width
        ) {

        player->action = MOVE;
        vectorDirection = Vector2Subtract(GetMousePosition(), player->position);   
        }
    if (player->action == MOVE){
        player->direction = Vector2LineAngle(
                                Vector2Zero(),
                                vectorDirection
                            ) * RAD2DEG * -1.0f;
        player->direction += 90.0f;
    }


return true;
}

void updatePlayer(){

// Condições de vitória ou derrota.
        if (player->life < 1) {
            currentSceneType = OVER;
            return;
        }

        // Verifica o nível atual
        if (score > level * 20) {
            enemyVel += 1;
            level++;
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
            currentItem->life--;

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

            // todo REFATORAR ESSE CÓDIGO PARA COLOCAR EM UMA FUNÇÃO
            // criar item novo e remover anterior
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

                ItemType newItemType = (ItemType) GetRandomValue(CHEESE, TRAP);
                newNode->obj = getItem(
                    (Vector2){
                        (float) GetRandomValue(64, SCREEN_WIDTH - 64), (float) (float) GetRandomValue(64, SCREEN_HEIGHT - 64)
                    },
                    &itemSpriteSheet[newItemType],
                    newItemType
                );

                newNode->next = currentNode->next;
                currentNode->next = newNode;
            }
        }

}

void updateEnemies(){
        // ------------------------------------------------------------
        // "ações" dos INIMIGOS
        for (ActorNode *currentNode = enemyListHead; currentNode != NULL; currentNode = currentNode->next) {
            // verifica se está vivo
            if (currentNode->obj->life < 1) continue;

            // zera variáveis
            currentActor = currentNode->obj;
            currentActor->action = STOP;
            currentActor->speed = enemyVel;// ação padrão


            // se estiver distante do jogador então permanece parado
            if (Vector2Distance(currentActor->position, player->position) > 200 ) continue;

            // se estiver próximo do jogador então comportamento de perseguição
            if (Vector2Distance(currentActor->position, player->position) > 64 && currentActor->action == STOP) {

                currentActor->direction = Vector2LineAngle(
                                        currentActor->position,
                                        player->position) * RAD2DEG * -1;
                currentActor->direction += 90.0f;
            }

            // move action
            actionMove(currentActor,  &arena);

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

    updatePlayer();
    updateEnemies();
    updateItens();

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
        DrawRectangle(150,20,player->life*2,20,LIME);
        DrawRectangleLines(150,20,200,20,BLACK);
        DrawText(TextFormat("VIDA : %d", player->life), 20, 20, 20, BLACK);
        DrawText(TextFormat("PONTOS: %d", score), 20, 40, 20, BLACK);
        DrawText(TextFormat("NÍVEL: %d", level), 500, 20, 40, BLACK);

        // debug infor
        if (debugMode) {
            BeginBlendMode(BLEND_ALPHA);
            DrawRectangle(15, 450, 270, 140, ColorAlpha(SKYBLUE, 0.5f));
            DrawText( TextFormat("Player->direction: %f", player->direction), 20, 560, 18, BLACK);
            DrawText( TextFormat("Player->Action: %d", player->action), 20, 540, 18, BLACK);
            DrawText( TextFormat("SpriteA->repeat: %d", player->spriteA2[player->action]->repeat), 20, 520, 18, BLACK);
            DrawText( TextFormat("Player Pos: x %d y %d", (int) player->position.x, (int) player->position.y ), 20, 500, 18, BLACK);
            DrawText( TextFormat("GriMap Pos: i %d j %d", (int) floorf(player->position.x/TILE_SIZE), (int) floorf(player->position.y/TILE_SIZE)), 20, 480, 18,BLACK);
            DrawText( TextFormat("Mouse: %d %d", GetMouseX(), GetMouseY()), 20, 460, 18, BLACK);
            DrawText(TextFormat("FPS: %d", GetFPS() ), 500, 570, 20, BLACK);
            EndBlendMode();
        }

         EndDrawing();


    }

void saveGame(){

    // carregar pontuação salva
    FILE* file = fopen("score.dat", "rb");
    int top_5[5];

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
    file = fopen("score.dat","wb");
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
    free(itemSpriteSheet);

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
