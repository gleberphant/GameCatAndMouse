
#include "lists.h"

/*
inicia uma lista de OBJETOS - inimigos ou itens
recebe a posição inicial de cada objeto e o sprite dos objetos
*/ 
ActorNode* initActorList( Vector2 initPosition[], const char* sprite, short maxNodes)
{
    ActorNode *newListNode=NULL, *headListNode = NULL;
    Texture2D* spriteSheet = malloc( sizeof(Texture2D) * 4 );

    spriteSheet[0] = LoadTexture( TextFormat("%s%s", sprite, "_walk.png"));
    spriteSheet[1] = LoadTexture( TextFormat("%s%s", sprite, "_stop.png"));
    spriteSheet[2] = LoadTexture( TextFormat("%s%s", sprite, "_special.png"));
    spriteSheet[3] = (Texture2D)  { 0 };

    for (int i = 0 ; i < maxNodes; i++){
        newListNode = malloc(sizeof(ActorNode));
        newListNode->obj = malloc(sizeof(Actor));
        setActor(newListNode->obj, initPosition[i], spriteSheet);
        newListNode->next = headListNode;
        headListNode = newListNode;       
    }
    return headListNode;
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

ItemNode* initItemList( Vector2 initPosition[], const char* sprite, short maxNodes)
{
    ItemNode *newListNode=NULL, *headListNode = NULL;
  
    for (int i = 0 ; i < maxNodes; i++){
        newListNode = malloc(sizeof(ItemNode));
        newListNode->obj = malloc(sizeof(Item));
        newListNode->obj = getItem(initPosition[i], sprite, CHEESE);
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

void unloadActorList(ActorNode* targetList) {
    ActorNode *currentNode = targetList;
    while(currentNode != NULL) {
        ActorNode *nextNode = currentNode->next;
        free(currentNode);
        currentNode = nextNode;
    }
    // // Forma de fazer 2
    // for(ActorNode *cur = targetList, *prev = NULL; cur != NULL; prev = cur, cur = cur->next){
    //     free(prev);
    // }
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