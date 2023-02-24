#ifndef __BUBBLES_H__
#define __BUBBLES_H__

#include "gfc_vector.h"
#include "Entity.h"

Entity* Bubbles_New(Vector2D position, Vector2D gridPosition, Vector2D attackTargetGridPosition);

void Bubbles_Remove_From_list(List* bubbles_list, Entity* self);

#endif
