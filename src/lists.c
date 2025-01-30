#include "lists.h"

#include <stdio.h>
#include <stdlib.h>

/*
inicia uma lista de OBJETOS - inimigos ou itens
recebe a posição inicial de cada objeto e o sprite dos objetos
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


ActorNode* addActorNode(ActorNode *oldHead) {

    ActorNode *newHead = malloc(sizeof(ActorNode));

    if (newHead == NULL) {
        TraceLog(LOG_ERROR, "::: Falha ao alocar memoria para ACTOR NODE \n");
        return oldHead;
    }

    newHead->next = oldHead;

    return newHead;
}



void drawActorList(ActorNode* targetList){
    for(ActorNode* currentNode = targetList; currentNode != NULL ; currentNode = currentNode->next)
    {
        if (currentNode->obj->life < 1){
                continue;
        }
        drawActor(currentNode->obj);
    }
      
}

void unloadActorList(ActorNode* targetList) {
    ActorNode *currentNode = targetList;
    while(currentNode != NULL) {
        ActorNode *nextNode = currentNode->next;
        free(currentNode->obj);
        free(currentNode);
        currentNode = nextNode;
    }
}

ItemNode* addItemNode(ItemNode* oldHead){
    ItemNode *newHead = malloc( sizeof(ItemNode) );
    newHead->next = oldHead;

    return newHead;
}


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



void unloadItemList(ItemNode* targetList) {
    ItemNode *currentNode = targetList;
    while(currentNode != NULL) {
        ItemNode *nextNode = currentNode->next;
        free(currentNode->obj);
        free(currentNode);
        currentNode = nextNode;
    }
}




void removeItemNode(ItemNode *target){
    ItemNode *oldNode=NULL;

    oldNode = target->next;

    if(target->next  != NULL){
        target->next = target->next->next;
        
        free(oldNode);
    }
}


void drawItemList(ItemNode* targetList){
    for(ItemNode* currentNode = targetList; currentNode != NULL ; currentNode = currentNode->next)
    {
        if (currentNode->obj->life < 1){
                continue;
        }

        drawItem(currentNode->obj);
    }
}

