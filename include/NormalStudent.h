#ifndef __NORMAL_STUDENT_H__
#define __NORMAL_STUDENT_H__

#include "gfc_vector.h"
#include "entity.h"
#include "UniversalData.h"

/**
 * @brief spawn a new normal student at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* normal_student_new(Vector2D position, int gridPositionX, int gridPositionY);
Entity* turning_student_new(Vector2D position, int gridPositionX, int gridPositionY);
Entity* distracted_student_new(Vector2D position, int gridPositionX, int gridPositionY, int index);
Entity* nervous_student_new(Vector2D position, int gridPositionX, int gridPositionY, Entity* playerEnt);
Entity* lost_student_new(Vector2D position, int gridPositionX, int gridPositionY, int targetGridX, int targetGridY, int index);



void normal_student_remove_from_list(List *student_list, Entity *self);

#endif