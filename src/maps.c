#include "maps.h"




// verifica se um objeto está dentro de um retangulo.
bool isInside(Actor* target, Rectangle *arena){
        
    if( target->position.x < arena->x || target->position.x > (arena->width + arena->x)){
        return false;
    }
    if( target->position.y < arena->y || target->position.y > (arena->height + arena->y)){
        return false;
    }
    return true;
}


   Vector2* getEnemysPosition(){


    static Vector2 temp[3]; 

    temp[0] = (Vector2){20.0f, 140.0f};
    temp[1] = (Vector2){650.0f, 120.0f};
    temp[2] = (Vector2){80.0f, 420.0f};
    
    return temp;
    }



    Vector2* getItensPosition(){  

    static Vector2 temp[3]; 
    
    temp[0] = (Vector2){100.0f, 120.0f};
    temp[1] = (Vector2){100.0f, 120.0f};
    temp[2] = (Vector2){300.0f, 400.0f};


    return temp;
}