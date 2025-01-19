#ifndef LIST_H_

#define LIST_H_


#include "raylib.h"


#include "itens.h"
#include "maps.h"
#include "actor.h"


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


ActorNode* getActorList( Vector2 initPosition[], const char* sprite, short maxNodes);

void drawActorList(ActorNode* targetList);

void unloadActorList(ActorNode* targetList);

// itens

ItemNode* getItemList( MapItens initItens[], Texture2D* spritesheet, short maxNodes);
void addItemNode(ItemNode** target);
void removeItemNode(ItemNode *target);
void drawItemList(ItemNode* targetList);
void unloadItemList(ItemNode* targetList);


#endif