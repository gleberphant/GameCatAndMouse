#ifndef LIST_H_

#define LIST_H_


#include "raylib.h"

#include "actor.h"
#include "itens.h"


// LINKED LIST DE ATORES
typedef struct ActorNode{
    Actor* obj;
    struct ActorNode* next;
    struct ActorNode* prev;
}ActorNode;


typedef struct ItemNode{
    Item* obj;
    struct ItemNode* next;
    struct ItemNode* prev;
}ItemNode;


ActorNode* initActorList( Vector2 initPosition[], char* sprite, short maxNodes);

void drawActorList(ActorNode* targetList);


ItemNode* initItemList( Vector2 initPosition[], char* sprite, short maxNodes);

void drawItemList(ItemNode* targetList);

#endif