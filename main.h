#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "actor.h"
#include "lists.h"
#include "maps.h"

// ESTADOS QUE O JOGO PODE SE ENCONTRAR
typedef enum  GameStatus{ 
    INTRO, 
    MENU, 
    GAME, 
    OVER,
    EXIT 
} GameStatus;