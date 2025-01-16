
#include "lists.h"


/* 
inicia uma lista de OBJETOS - inimigos ou itens
recebe a posição inicial de cada objeto e o sprite dos objetos
*/ 
LinkedNode* initLinkedList( Vector2 initPosition[], char* sprite, short maxNodes)
{
    LinkedNode *newListNode, *headListNode = NULL;
  
    for (int i = 0 ; i < maxNodes; i++){
        newListNode = malloc(sizeof(LinkedNode));
        newListNode->obj = malloc(sizeof(Object));
        setObject(newListNode->obj, initPosition[i], sprite);
        newListNode->next = headListNode;
        headListNode = newListNode;       
    }
    return headListNode;
}


void drawNodeList(LinkedNode* targetList){
    for(LinkedNode* currenteNode = targetList; currenteNode != NULL ; currenteNode = currenteNode->next)
    {
        if (currenteNode->obj->life < 1){
                continue;
        }

        drawObject(currenteNode->obj);
    }
      
}

ItemNode* initItemList( Vector2 initPosition[], char* sprite, short maxNodes)
{
    ItemNode *newListNode, *headListNode = NULL;
  
    for (int i = 0 ; i < maxNodes; i++){
        newListNode = malloc(sizeof(ItemNode));
        newListNode->obj = malloc(sizeof(Item));
        newListNode->obj = getItem(initPosition[i], sprite, CHEESE);
        newListNode->next = headListNode;
        headListNode = newListNode;       
    }
    return headListNode;
}


void drawItemList(ItemNode* targetList){
    for(ItemNode* currenteNode = targetList; currenteNode != NULL ; currenteNode = currenteNode->next)
    {
        if (currenteNode->obj->life < 1){
                continue;
        }

        drawItem(currenteNode->obj);
    }
      
}