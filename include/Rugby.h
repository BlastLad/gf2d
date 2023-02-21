#ifndef  __RUGBY_H__
#define __RUGBY_H__

#include "gfc_vector.h"
#include "Entity.h"

Entity* Rugby_New(Vector2D position, Vector2D gridPosition);

void Rugby_remove_from_list(List* rugby_list, Entity* self);

#endif
