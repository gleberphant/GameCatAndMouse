#include "lists.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Carrega uma lista de atores.
 * 
 * Esta função carrega uma lista de atores (inimigos ou itens) a partir de um arquivo.
 * 
 * @param mapfilePath O caminho para o arquivo do mapa.
 * @param spriteSheet O spritesheet dos atores.
 * @return ActorNode* Retorna um ponteiro para a cabeça da lista de atores carregada.
 */
ActorNode* loadActorList(const char* mapfilePath, Texture2D* spriteSheet)
{
    ActorNode *headListNode = NULL;

    FILE *filemap = fopen(mapfilePath, "r");
    if (filemap == NULL) {
        TraceLog(LOG_DEBUG, "Failed to load enemy layer \n");
        return headListNode;
    }

    char tileValue;
    for (int row=0; row< NUM_TILES_HEIGHT; row++) {
        for (int col=0; col< NUM_TILES_WIDTH; col++) {

            fscanf(filemap, "%c,", &tileValue );

            if (tileValue == '1') {
                headListNode = addActorNode(headListNode);
                headListNode->obj = loadNewActor(
                    (Vector2){(float) col * TILE_SIZE, (float) row * TILE_SIZE},
                    spriteSheet
                );
            }
        }
    }
    fclose(filemap);
    return headListNode;
}

/**
 * @brief Adiciona um nó de ator à lista.
 * 
 * Esta função adiciona um novo nó de ator à cabeça da lista.
 * 
 * @param oldHead A cabeça antiga da lista.
 * @return ActorNode* Retorna um ponteiro para a nova cabeça da lista.
 */
ActorNode* addActorNode(ActorNode *oldHead) {

    ActorNode *newHead = malloc(sizeof(ActorNode));

    if (newHead == NULL) {
        TraceLog(LOG_ERROR, "::: Falha ao alocar memoria para ACTOR NODE \n");
        return oldHead;
    }

    newHead->next = (oldHead == NULL) ? NULL : oldHead;
    

    return newHead;
}

/**
 * @brief Desenha a lista de atores.
 * 
 * Esta função renderiza todos os atores na lista.
 * 
 * @param targetList A lista de atores a ser desenhada.
 */
void drawActorList(ActorNode* targetList){
    for(ActorNode* currentNode = targetList; currentNode != NULL ; currentNode = currentNode->next)
    {
        if (currentNode->obj->life < 1){
                continue;
        }
        drawActor(currentNode->obj);
    }
      
}

/**
 * @brief Descarrega a lista de atores.
 * 
 * Esta função libera os recursos alocados para a lista de atores.
 * 
 * @param targetList A lista de atores a ser descarregada.
 */
void unloadActorList(ActorNode* targetList) {
    ActorNode *currentActorNode = targetList;
    while(currentActorNode != NULL) {
        ActorNode *nextNode = currentActorNode->next;
        TraceLog(LOG_DEBUG, "== liberando ACTOR %p", currentActorNode);
        free(currentActorNode->obj);
        free(currentActorNode);
        currentActorNode = nextNode;
    }
}

/**
 * @brief Adiciona um nó de item à lista.
 * 
 * Esta função adiciona um novo nó de item à cabeça da lista.
 * 
 * @param oldHead A cabeça antiga da lista.
 * @return ItemNode* Retorna um ponteiro para a nova cabeça da lista.
 */
ItemNode* addItemNode(ItemNode* oldHead){
    ItemNode *newHead = malloc( sizeof(ItemNode) );
    newHead->next = oldHead;

    return newHead;
}

/**
 * @brief Carrega uma lista de itens.
 * 
 * Esta função carrega uma lista de itens a partir de um arquivo.
 * 
 * @param spritesheet O spritesheet dos itens.
 * @return ItemNode* Retorna um ponteiro para a cabeça da lista de itens carregada.
 */
ItemNode* loadItemList(Texture2D* spritesheet)
{
    ItemNode *headListNode = NULL;

    // carregar tile map do arquivo
    FILE* filemap = fopen("resources/itemLayer01.data", "r");

    if (filemap == NULL) {
        TraceLog(LOG_DEBUG, "Failed to load map\n");
    }

    int tileValue;
    for (int row = 0; row < NUM_TILES_HEIGHT; row++){
        for (int col = 0; col < NUM_TILES_WIDTH; col++) {
            fscanf(filemap, "%d,", &tileValue);

            if (tileValue != 9) {

                headListNode = addItemNode(headListNode);

                headListNode->obj = loadNewItem(
                    (Vector2){col*TILE_SIZE, row*TILE_SIZE},
                    spritesheet,
                    tileValue);
            }
        }
    }

    fclose(filemap);

    return headListNode;

}

/**
 * @brief Descarrega a lista de itens.
 * 
 * Esta função libera os recursos alocados para a lista de itens.
 * 
 * @param targetList A lista de itens a ser descarregada.
 */
void unloadItemList(ItemNode* targetList) {
    ItemNode *currentNode = targetList;

    while(currentNode != NULL) {
        ItemNode *nextNode = currentNode->next;
        TraceLog(LOG_DEBUG, "== liberando ITEM %p", currentNode);
        free(currentNode->obj);
        free(currentNode);
        currentNode = nextNode;
    }
}

/**
 * @brief Remove um nó de item da lista.
 * 
 * Esta função remove um nó de item da lista.
 * 
 * @param target O nó de item a ser removido.
 */
void removeItemNode(ItemNode *target){
    ItemNode *oldNode=NULL;

    if(target->next  != NULL){
        oldNode = target->next;
        target->next = target->next->next;
        
        free(oldNode);
    }
}

/**
 * @brief Desenha a lista de itens.
 * 
 * Esta função renderiza todos os itens na lista.
 * 
 * @param targetList A lista de itens a ser desenhada.
 */
void drawItemList(ItemNode* targetList){
    for(ItemNode* currentNode = targetList; currentNode != NULL ; currentNode = currentNode->next)
    {
        if (currentNode->obj->life < 1){
                continue;
        }

        drawItem(currentNode->obj);
    }
}

