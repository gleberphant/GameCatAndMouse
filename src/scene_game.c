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


Actor *player = NULL;
ActorNode *enemyListHead = NULL, *playerListHead = NULL;
ItemNode *itemListHead = NULL;

float volumeMaster = 1.00f;
int level=0, score=0, enemyVel=4;

Texture2D *itemSpriteSheetArray, *catSpriteSheetArray, *mouseSpriteSheetArray;
Sound eatCheese, eatStrawberry, getHit;

short enemyType = 0, enemyCount = 0;
Vector2 targetPosition;


/**
 * @brief Inicializa a cena do jogo.
 * 
 * Esta função carrega os recursos necessários para a cena do jogo e define as funções de entrada, atualização e desenho.
 * 
 * @return SceneData* Retorna um ponteiro para os dados da cena inicializada.
 */
SceneData* initSceneGame(){

    // zera todos ponteiros
    player = NULL;
    enemyListHead = NULL, playerListHead = NULL, itemListHead = NULL;

    // CARREGAR LISTA DE INIMIGOS
    TraceLog(LOG_DEBUG, "== carregando ENEMY LIST");

    catSpriteSheetArray = loadActorSpriteSheetArray("resources/catA");

    enemyListHead = loadActorList( "resources/enemyLayer01.data", catSpriteSheetArray);

    if (enemyListHead == NULL) {
        TraceLog(LOG_ERROR, " ::: ERROR ao carregar ENEMY LIST");
        currentSceneType = EXIT;
    }

    // Carregar personagem do jogador
    TraceLog(LOG_DEBUG, "== carregando PLAYER");

    mouseSpriteSheetArray = loadActorSpriteSheetArray("resources/mouseA");

    playerListHead = addActorNode(NULL);

    if (playerListHead == NULL) {
        TraceLog(LOG_ERROR, " ::: ERROR ao carregar PLAYER list");
        currentSceneType = EXIT;
        return NULL;
    }

    playerListHead->obj = loadNewActor(
        (Vector2){(float) 4 * TILE_SIZE, (float) 4 * TILE_SIZE},
        mouseSpriteSheetArray
    );
    player = playerListHead->obj;

    if (player == NULL) {
        TraceLog(LOG_ERROR, " ::: ERROR ao carregar PLAYER list");
        currentSceneType = EXIT;
        return NULL;
    }

    player->speed = 10;
    player->life = 90;


    // CARREGAR LISTA DE ITENS
    TraceLog(LOG_DEBUG, "== carregando ITENS LIST");
    const char *itemSpritePathList[] = {
        "resources/item_cheese.png",
        "resources/item_strawberry.png",
        "resources/item_trap.png"
    };

    itemSpriteSheetArray = loadItemSpriteSheetArray(itemSpritePathList);

    itemListHead = loadItemList(
        itemSpriteSheetArray
    );

    if (itemListHead == NULL) {
        TraceLog(LOG_ERROR, "::: ERROR ao carregar ITENS LIST");
        currentSceneType = EXIT;
    }


    // Inicializar pontuação e nível
    score = 0;
    level = 1;
    enemyVel = 5;


    // CARREGAR EFEITOS SONOROS
    TraceLog(LOG_DEBUG, "== carregando EFEITOS SONOROS");
    eatCheese = LoadSound("resources/sounds/eat_cheese.ogg");
    eatStrawberry = LoadSound("resources/sounds/eat_strawberry.ogg");
    getHit = LoadSound("resources/sounds/get_hit.ogg");


    TraceLog(LOG_DEBUG, "== carregando MAPA TILESET");
    loadMap("resources/tileset.png");

    TraceLog(LOG_DEBUG, "== INICIANDO SCENE");
    SceneData *scene = loadSceneData(
    "resources/bg.png",
    "resources/sounds/game_music.ogg",
    GAME,
    &gameFont
    );

    PlayMusicStream(scene->music);
    SetMusicVolume(scene->music, volumeMaster);

    SetExitKey(KEY_NULL);

    return scene;


}

/**
 * @brief Lida com a entrada do jogador.
 * 
 * Esta função processa a entrada do jogador, como teclas de movimento e ações especiais.
 * 
 * @return bool Retorna true se a entrada foi processada com sucesso, false caso contrário.
 */
bool handlePlayerInput(){

    if (IsKeyPressed(KEY_ESCAPE)) {
        currentSceneType = INTRO;
        return false;
    }

    // DEBUG OPTIONS
    if (IsKeyReleased(KEY_F1)) {
        debugMode = !debugMode;
    }

    if (IsKeyReleased(KEY_KP_ADD)) {
        volumeMaster += 0.05f;
        SetMasterVolume(volumeMaster);
    }

    if (IsKeyReleased(KEY_KP_SUBTRACT)) {
        volumeMaster -= 0.05f;
        SetMasterVolume(volumeMaster);
    }


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

/**
 * @brief Atualiza o mapa do jogo.
 * 
 * Esta função atualiza o mapa do jogo, incluindo a verificação do nível atual e a adição de novos inimigos.
 */
void updateMap() {
    // Verifica o nível atual
    if (score > level * 20) {
        enemyVel = enemyVel > 8 ? 8 : enemyVel+1;
        level++;


        ActorNode* newCatNode = malloc(sizeof(ActorNode));
        newCatNode->obj = malloc(sizeof(Actor));
        newCatNode->next = enemyListHead;
        enemyListHead = newCatNode;

        setActor(enemyListHead->obj,
            (Vector2){4*TILE_SIZE, 4*TILE_SIZE},
            catSpriteSheetArray);
    }
}

/**
 * @brief Atualiza o jogador.
 * 
 * Esta função atualiza a lógica do jogador, incluindo a verificação de condições de vitória ou derrota.
 */
void updatePlayer(){

// Condições de vitória ou derrota.
        if (player->life < 1) {
            currentSceneType = OVER;
        }

        // ações do jogador
        switch (player->action) {
            case STOP:
                actionStop(player);
                break;

            case MOVE:
                if (player->life == 100) player->speed = 15;
                else player->speed = 8;
                actionMove(player);
                break;

            case SPECIAL:
                actionSpecial(player, player);
                break;

            default:
                TraceLog(LOG_DEBUG, "item desconhecido");
                break;
        }

}

/**
 * @brief Atualiza os itens do jogo.
 * 
 * Esta função atualiza a lógica dos itens do jogo, incluindo a verificação de colisões com o jogador.
 */
void updateItens(){

    Item* currentItem = NULL;
        // ------------------------------------------------------------
        // "ações" dos iten
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
                        player->life = player->life+10 > 100 ? 100 : player->life+10;

                        break;

                    case TRAP:

                        currentItem->life = -1;
                        player->life -= 10;
                        player->action = SPECIAL;
                        break;
                    default:
                        TraceLog(LOG_DEBUG, "item desconhecido");
                        currentItem->life = -1;
                        break;
                }
            }

            if (currentItem->life < 1) {
                // REMOVER O NÓ
                if (prev == NULL) {
                    prev = currentNode->next;
                    itemListHead = prev;
                } else prev->next = currentNode->next;

                free(currentNode->obj);
                free(currentNode);

                currentNode = prev;
                if (currentNode == NULL) break;

                // CRIA UM NOVO ITEM ALEATÓRIO
                itemListHead = addItemNode(itemListHead);

                itemListHead->obj = loadNewItem(
                    (Vector2){
                        (float) GetRandomValue(2, NUM_TILES_WIDTH  - 2)  * TILE_SIZE,
                        (float) GetRandomValue(2, NUM_TILES_HEIGHT - 2 ) * TILE_SIZE
                    },
                    itemSpriteSheetArray,
                    GetRandomValue(CHEESE, TRAP)
                );

                currentItem = itemListHead->obj;

                // verifica se o item foi criado embaixo do jogador
                if (CheckCollisionRecs(player->collisionBox, currentItem->collisionBox)) {
                    currentItem->collisionBox = getItemCollisionBox(
                        (Vector2){(float) GetRandomValue(64, SCREEN_WIDTH - 64), (float) (float) GetRandomValue(64, SCREEN_HEIGHT - 64)}
                    );

                    currentItem->position = (Vector2) {
                        currentItem->collisionBox.x + currentItem->collisionBox.width / 2,
                        currentItem->collisionBox.y + currentItem->collisionBox.height /2 };

                }

            }

            // dimiinuir vida do item
            currentItem->life--;
        }

}

/**
 * @brief Atualiza os inimigos do jogo.
 * 
 * Esta função atualiza a lógica dos inimigos do jogo, incluindo a verificação de colisões com o jogador.
 */
void updateEnemies(){

    Actor* currentActor = NULL;
    enemyCount = 0;
        // ------------------------------------------------------------

        // "ações" dos INIMIGOS
        for (ActorNode *currentNode = enemyListHead; currentNode != NULL; currentNode = currentNode->next) {
            // verifica se está vivo
            if (currentNode->obj->life < 1) continue;
            else enemyCount++;
            
            // zera variáveis
            currentActor = currentNode->obj;

            currentActor->action = STOP;
            currentActor->speed =  GetRandomValue(1 ,  enemyVel); // velocidade variável
            currentActor->count ++;
            targetPosition = player->position;

            if (currentActor->count > 180){
                currentActor->behavior = GetRandomValue(ATTACK, CRAZY);
                currentActor->count = 0 ;

            }

            // chose behavior
            switch (currentActor->behavior){
                // persegue o jogador independente de qualquer coisa
                case ATTACK:
                    targetPosition = player->position;
                    currentActor->action = MOVE;
                    break;

                case GUARD_CHEESE:
                    // protege o primeiro queijo que encontrar na lista quando player não estiver muito próximo
                    if ( Vector2Distance(currentActor->position, player->position) > 128 ){
                        for (ItemNode *currentItem = itemListHead; currentItem != NULL; currentItem = currentItem->next) {
                            if (currentItem->obj->type == CHEESE ) {
                                targetPosition = currentItem->obj->position;
                                currentActor->action =  Vector2Distance(currentActor->position, targetPosition) < 32 ? STOP : MOVE;
                                break;
                            }
                        }
                    } else{ currentActor->action = MOVE;}
                    break;

                case GUARD_STRAW:
                    // protege o primeiro morango que encontrar na lista quando player não estiver muito próximo.
                    if ( Vector2Distance(currentActor->position, player->position) > 128 ){
                        for (ItemNode *currentItem = itemListHead; currentItem != NULL; currentItem = currentItem->next) {
                            if ( currentItem->obj->type == STRAWBERRY) {
                                targetPosition = currentItem->obj->position;
                                currentActor->action =  Vector2Distance(currentActor->position, targetPosition) < 32 ? STOP : MOVE;
                                break;
                            }
                        }
                    }else{ currentActor->action = MOVE;}
                    break;

                case CRAZY:
                    currentActor->action =  MOVE;
                    targetPosition.x = player->position.x * cosf(DEG2RAD * 45);
                    targetPosition.y = player->position.y * sinf(DEG2RAD * 45);

                    break;
                case SLEEPER:
                default:// comportamento de dormir se jogador estiver longe
                    if (Vector2Distance(currentActor->position, player->position) < 500 || player->action == SPECIAL ) {
                        currentActor->action = MOVE;
                    }
                    break;
            };


            // do actions
            switch(currentActor->action){
                case MOVE:
                    currentActor->direction =
                    Vector2Distance(currentActor->position, targetPosition) > 64 ?
                        Vector2LineAngle( currentActor->position, targetPosition) * -1 :
                            currentActor->direction;
                                        
                    actionMove(currentActor);

                    break;
                
                default:
                    actionStop(currentActor);
                    break;
            };

            // Check collisions
            if (CheckCollisionRecs(player->collisionBox, currentActor->collisionBox)) {
                PlaySound(getHit);
                currentActor->collision = true;
                currentActor->pointOfCollision = GetCollisionRec(player->collisionBox, currentActor->collisionBox);
                
                if (!debugMode) player->life-= 8;
                
            }
        }
}

/**
 * @brief Atualiza a cena do jogo.
 * 
 * Esta função atualiza a lógica da cena do jogo, incluindo o mapa, o jogador, os itens e os inimigos.
 */
void updateSceneGame(){
    updateMap();
    updatePlayer();
    updateEnemies();
    updateItens();
}

/**
 * @brief Desenha o HUD do jogo.
 * 
 * Esta função desenha o HUD (Heads-Up Display) do jogo, incluindo a vida do jogador, a pontuação e o nível.
 */
void drawHud() {
    
    // desenha dialog do hud
    DrawRectangle(20, 20, 380, 50, ColorAlpha(SKYBLUE, 0.5f));

    // desenha fundo da barra de vida
    DrawRectangle(149,19, 202 ,22,BLACK);

    // desenha barra de vida com a cor conforme o percentual de vida
    DrawRectangle(
        150, 20,
        player->life*2,20, 
        player->life == 100 ? SKYBLUE : player->life > 49 ? LIME : player->life>= 20? ORANGE: RED
    );
    
    
    
    if(player->life == 100) DrawText(TextFormat("VIDA : MAX"), 20, 20, 20, BLACK);
    else DrawText(TextFormat("VIDA : %d", player->life), 20, 20, 20, BLACK);  

    DrawText(TextFormat("PONTOS: %d", score), 20, 40, 20, BLACK);
    DrawText(TextFormat("NÍVEL: %d", level), 750, 20, 50, BLACK);
}

/**
 * @brief Desenha informações de depuração.
 * 
 * Esta função desenha informações de depuração na tela, como a posição do jogador e a taxa de quadros por segundo (FPS).
 */
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

/**
 * @brief Desenha a cena do jogo na tela.
 * 
 * Esta função renderiza a cena do jogo na tela.
 */
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
        drawActor(player);

        //desenha HUD
        drawHud();

        // debug infor
        if (debugMode) {
            drawDebug();
        }

    EndDrawing();


}

/**
 * @brief Salva o jogo.
 * 
 * Esta função salva a pontuação do jogador em um arquivo.
 */
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

/**
 * @brief Fecha a cena do jogo.
 * 
 * Esta função libera os recursos alocados para a cena do jogo.
 */
void closeSceneGame(){

    //LIBERAR MEMORIA PLAYER
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA PLAYER LIST");
    unloadActorSpriteSheet(mouseSpriteSheetArray);
    unloadActorList(playerListHead);
    //free(player);
    //free(playerListHead);


    //LIBERAR MEMORIA ENEMY LIST
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA ENEMY LIST");
    unloadActorSpriteSheet(catSpriteSheetArray);
    unloadActorList(enemyListHead);


    //LIBERAR MEMORIA ITEM LIST
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA ITEM LIST");
    unloadItemSpriteSheet(itemSpriteSheetArray);
    unloadItemList(itemListHead);


    //LIBERAR MEMORIA MAPA
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA MAPA");

    //LIBERAR MEMORIA EFEITOS SONOROS
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA EFEITOS SONOROS");
    UnloadSound(eatCheese);
    UnloadSound(getHit);
    UnloadSound(eatStrawberry);

    //LIBERAR CENA
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA CENA");
    unloadScene(currentScene);

}

/**
 * @brief Executa o loop principal da cena do jogo.
 * 
 * Esta função executa o loop principal da cena do jogo, incluindo a entrada, atualização e desenho da cena.
 */
void loopSceneGame(){
    while (currentSceneType == GAME) {
        
        if (!handlePlayerInput()) {
            currentSceneType = INTRO;
            break;
        }

        updateSceneGame();

        drawSceneGame();

        UpdateMusicStream(currentScene->music);
    }

}
// cena loop principal
/**
 * @brief Executa a cena do jogo.
 * 
 * Esta função inicializa, executa e finaliza a cena do jogo.
 */
void runSceneGame() {

    currentScene = initSceneGame();

    loopSceneGame();

    StopMusicStream(currentScene->music);

    closeSceneGame();

    saveGame();
}
//----------------------------------------------------------
// FIM DO ARQUIVO
