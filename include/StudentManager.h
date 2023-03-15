#ifndef __STUDENT_MANAGER_H__
#define __STUDENT_MANAGER_H__

#include "gfc_vector.h"

#include "NormalStudent.h"



Vector2D Determine_Spawn_Position();

Entity* Determine_Student_To_Spawn(int remainingStudents, Entity* playerEnt);





#endif
