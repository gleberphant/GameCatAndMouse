#include "maps.h"


// verifica se um objeto esta dentro de outro
bool isInside(Object* target, Rectangle *arena){
        
    if( target->position.x < arena->x || target->position.x + target->box.width > (arena->width + arena->x)){
        return false;
    }
    if( target->position.y < arena->y || target->position.y + target->box.height > (arena->height + arena->y)){
        return false;
    }
    return true;
}