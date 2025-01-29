#include "lists.h"

#include <stdlib.h>

/*
inicia uma lista de OBJETOS - inimigos ou itens
recebe a posição inicial de cada objeto e o sprite dos objetos
*/ 
ActorNode* getActorList( Vector2 initPosition[], Texture2D* spriteSheet, short maxNodes)
{
    ActorNode *newListNode=NULL, *headListNode = NULL;

    for (int i = 0 ; i < maxNodes; i++){
        newListNode = malloc(sizeof(ActorNode));
        newListNode->obj = malloc(sizeof(Actor));
        setActor(newListNode->obj, initPosition[i], spriteSheet);
        newListNode->next = headListNode;
        headListNode = newListNode;       
    }

    return headListNode;
}


Texture2D* getSpriteSheet( const char* sprite){

    Texture2D* spriteSheet = malloc( sizeof(Texture2D) * 4 );

    spriteSheet[STOP]    = LoadTexture( TextFormat("%s%s", sprite, "_stop.png"));
    spriteSheet[MOVE]    = LoadTexture( TextFormat("%s%s", sprite, "_walk.png"));
    spriteSheet[SPECIAL] = LoadTexture( TextFormat("%s%s", sprite, "_special.png"));
    spriteSheet[END]     = (Texture2D)  { 0 };

    return spriteSheet;

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
        free(currentNode);
        currentNode = nextNode;
    }
}


ItemNode* getItemList( InitListItens initItens[], Texture2D* spritesheet, short maxNodes)
{
    ItemNode *newListNode=NULL, *headListNode = NULL;


    // criar nós
    for (int i = 0 ; i < maxNodes; i++){
        newListNode = malloc(sizeof(ItemNode));
        newListNode->obj = malloc(sizeof(Item));

        newListNode->obj = getItem(
            initItens[i].initPos,
            spritesheet,
            initItens[i].type);

        newListNode->next = headListNode;
        headListNode = newListNode;       
    }
    return headListNode;
}


void addItemNode(ItemNode** target){
    ItemNode *newNode = malloc(sizeof(ItemNode));
    newNode->next= *target;
    *target = newNode;
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


void unloadItemList(ItemNode* targetList) {
    ItemNode *currentNode = targetList;
    while(currentNode != NULL) {
        ItemNode *nextNode = currentNode->next;
        free(currentNode);
        currentNode = nextNode;
    }
}
