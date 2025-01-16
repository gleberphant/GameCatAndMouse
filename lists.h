#ifndef LIST_H_

#define LIST_H_


#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>

#include "actor.h"
#include "itens.h"


// LINKED LIST DE ATORES
typedef struct LinkedNode{
    Object* obj;
    struct LinkedNode* next;
}LinkedNode;


typedef struct ItemNode{
    Item* obj;
    struct ItemNode* next;
}ItemNode;



LinkedNode* initLinkedList( Vector2 initPosition[], char* sprite, short maxNodes);

void drawNodeList(LinkedNode* targetList);


ItemNode* initItemList( Vector2 initPosition[], char* sprite, short maxNodes);

void drawItemList(ItemNode* targetList);

#endif