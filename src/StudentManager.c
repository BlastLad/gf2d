#include "StudentManager.h"
#include "TileMap.h"
Vector2D Determine_Spawn_Position() 
{
	float randNum;
	randNum = gfc_random();
	if (randNum > 0.5) {
		return vector2d(7, 1);
	}
	else if (randNum > 0.5) {
		return vector2d(11, 1);
	}
}


Entity* Determine_Student_To_Spawn() {
	float randNum;
	randNum = gfc_random();

	if (randNum > 0.5) {
		randNum = gfc_random();

		if (randNum > 0.5) {
			return turning_student_new(graph_to_world_pos(7, 1), 7, 1);
		}
		else if (randNum > -0.1) {
			return turning_student_new(graph_to_world_pos(11, 1), 11, 1);
		}
	}
	else if (randNum > -0.1) {
		randNum = gfc_random();

		if (randNum > 0.5) {
			return normal_student_new(graph_to_world_pos(7, 1), 7, 1);
		}
		else if (randNum > -0.1) {
			return normal_student_new(graph_to_world_pos(11, 1), 11, 1);
		}
	}

}

Entity* Turning_Behaviour() 
{
	float randNum;
	randNum = gfc_random();

	if (randNum > 0.5) {
		return turning_student_new(graph_to_world_pos(7, 1), 7, 1);
	}
	else if (randNum > -0.1) {
		return turning_student_new(graph_to_world_pos(11, 1), 11, 1);
	}
}

Entity* Normal_Behaviour()
{
	float randNum;
	randNum = gfc_random();

	if (randNum > 0.5) {
		return normal_student_new(graph_to_world_pos(7, 1), 7, 1);
	}
	else if (randNum > -0.1) {
		return normal_student_new(graph_to_world_pos(11, 1), 11, 1);
	}
}
