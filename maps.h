#ifndef MAPS_H_

#define MAPS_H_


#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "actor.h"
#include "lists.h"



// TODO : ESTRUTURA DE MAPA 
typedef struct Map{
    Rectangle area;
    Color color;
    LinkedNode *enemyList;
    LinkedNode *itensList;
} Map;



#endif