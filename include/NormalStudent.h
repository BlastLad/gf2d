#ifndef __NORMAL_STUDENT_H__
#define __NORMAL_STUDENT_H__

#include "gfc_vector.h"
#include "entity.h"

/**
 * @brief spawn a new normal student at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* normal_student_new(Vector2D position, Vector2D gridPosition);



void normal_student_remove_from_list(List *student_list, Entity *self);

#endif