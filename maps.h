#ifndef MAPS_H_

#define MAPS_H_


#include "raylib.h"
#include "raymath.h"


#include "actor.h"
#include "lists.h"

// TODO : ESTRUTURA DE MAPA 
typedef struct Map{
    Rectangle area;
    Color color;
    LinkedNode *enemyList;
    ItemNode *itensList;
} Map;



bool isInside(Object* target, Rectangle *arena);

#endif