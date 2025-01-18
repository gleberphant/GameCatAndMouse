
#include "lists.h"

/*
inicia uma lista de OBJETOS - inimigos ou itens
recebe a posição inicial de cada objeto e o sprite dos objetos
*/ 
ActorNode* initActorList( Vector2 initPosition[], char* sprite, short maxNodes)
{
    ActorNode *newListNode=NULL, *headListNode = NULL;
  
    for (int i = 0 ; i < maxNodes; i++){
        newListNode = malloc(sizeof(ActorNode));
        newListNode->obj = malloc(sizeof(Actor));
        setActor(newListNode->obj, initPosition[i], sprite);
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

ItemNode* initItemList( Vector2 initPosition[], char* sprite, short maxNodes)
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

void drawItemList(ItemNode* targetList){
    for(ItemNode* currentNode = targetList; currentNode != NULL ; currentNode = currentNode->next)
    {
        if (currentNode->obj->life < 1){
                continue;
        }

        drawItem(currentNode->obj);
    }
      
}