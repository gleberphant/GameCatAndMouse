#ifndef LIST_H_

#define LIST_H_


#include "raylib.h"


#include "itens.h"
#include "maps.h"
#include "actor.h"


// LINKED LIST
// NODES atores
typedef struct ActorNode{
    Actor* obj;
    struct ActorNode* next;
    struct ActorNode* prev;
}ActorNode;

ActorNode* loadActorList(const char* mapfilePath, Texture2D* spriteSheet);
ActorNode* addActorNode(ActorNode *oldHead);
void drawActorList(ActorNode* targetList);
void unloadActorList(ActorNode* targetList);
ActorNode* addActorNode(ActorNode *oldHead);


// NODES itens
typedef struct ItemNode{
    Item* obj;
    struct ItemNode* next;
    struct ItemNode* prev;
}ItemNode;


ItemNode* loadItemList(Texture2D* spritesheet);
ItemNode* addItemNode(ItemNode* oldHead);
void removeItemNode(ItemNode *target);
void drawItemList(ItemNode* targetList);
void unloadItemList(ItemNode* targetList);


#endif