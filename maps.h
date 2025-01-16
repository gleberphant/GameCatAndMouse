#ifndef MAPS_H_

#define MAPS_H_


#include "raylib.h"
#include "raymath.h"


#include "lists.h"




// MAPS
Vector2 mapEnemys[] = {
    (Vector2){20.0f, 140.0f},
    (Vector2){650.0f, 120.0f},
    (Vector2){80.0f, 420.0f}
    
};

Vector2 mapItens[] = {
    (Vector2){100.0f, 120.0f},
    (Vector2){600.0f, 500.0f}
};



// TODO : ESTRUTURA DE MAPA 
typedef struct Map{
    Rectangle area;
    Color color;
    LinkedNode *enemyList;
    LinkedNode *itensList;
} Map;



#endif