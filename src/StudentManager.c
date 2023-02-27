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

	if (randNum > 0.75) {
		randNum = gfc_random();

		if (randNum > 0.5) {
			return turning_student_new(graph_to_world_pos(7, 1), 7, 1);
		}
		else if (randNum > -0.1) {
			return turning_student_new(graph_to_world_pos(11, 1), 11, 1);
		}
	}
	else if (randNum > 0.5) {
		randNum = gfc_random();

		if (randNum > 0.5) {
			return normal_student_new(graph_to_world_pos(7, 1), 7, 1);
		}
		else if (randNum > -0.1) {
			return normal_student_new(graph_to_world_pos(11, 1), 11, 1);
		}
	}
	else if (randNum > 0.25) {
		randNum = gfc_random();
		float rand2;
		rand2 = gfc_random();
		if (randNum > 0.5) {
			if (rand2 > 0.5)
				return distracted_student_new(graph_to_world_pos(7, 1), 7, 1, 0);
			else 
				return distracted_student_new(graph_to_world_pos(7, 1), 7, 1, 1);
		}
		else if (randNum > -0.1) {
			if (rand2 > 0.5)
				return distracted_student_new(graph_to_world_pos(11, 1), 11, 1, 1);
			else 
				return distracted_student_new(graph_to_world_pos(11, 1), 11, 1, 0);

		}
	}
	else if (randNum > -0.1) 
	{
		randNum = gfc_random();
		float rand2;
		rand2 = gfc_random();
		if (randNum > 0.5) {
			if (rand2 > 0.5)
				return lost_student_new(graph_to_world_pos(7, 1), 7, 1, 7, 2, 1);
			else
				return lost_student_new(graph_to_world_pos(7, 1), 7, 1, 7, 2, 0);
		}
		else if (randNum > -0.1) {
			if (rand2 > 0.5)
				return lost_student_new(graph_to_world_pos(11, 1), 11, 1, 11, 2, 1);
			else
				return lost_student_new(graph_to_world_pos(11, 1), 11, 1, 11, 2, 0);

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
