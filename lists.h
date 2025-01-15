#ifndef LIST_H_

#define LIST_H_


#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "actor.h"

// LINKED LIST DE ATORES
typedef struct LinkedNode{
    Object* obj;
    struct LinkedNode* next;
}LinkedNode;


LinkedNode* initLinkedList( Vector2 initPosition[], char* sprite, short maxNodes);

void drawNodeList(LinkedNode* targetList);

#endif