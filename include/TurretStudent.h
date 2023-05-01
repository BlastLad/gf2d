#ifndef  __TURRET_STUDENT_H__
#define __TURRET_STUDENT_H__

#include "gfc_vector.h"
#include "Entity.h"

Entity* Turret_Student_New(Vector2D position, int gridPositionX, int gridPositionY);

void Turret_Student_remove_from_list(List* turret_student_list, Entity* self);

#endif
