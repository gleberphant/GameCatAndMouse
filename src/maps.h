#ifndef MAPS_H_

#define MAPS_H_


#include "raylib.h"

#include "actor.h"
#include "itens.h"


// TODO : ESTRUTURA DE MAPA 
typedef struct MapItens{
    ItemType type;
    Vector2 initPos;
} MapItens;


bool isInside(Actor* target, Rectangle *arena);

Vector2* getEnemysPosition();

Vector2* getItensPosition();

#endif