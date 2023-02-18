#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "Entity.h"
#include "NormalStudent.h"
#include "TileMap.h"


void normal_student_think(Entity *self);

Vector2D get_next_tile(int x, int y, int behaviourRule) {
	slog("before i");
	int i;
	slog("Broke he");
	for (int i = 0; i < 4; i++) {
		slog("Broke here");
		if (gfc_list_get_nth(get_graph_node(x, y).neighbours, i)) {
			slog("Broke there");
		}
	}

	return vector2d(get_graph_node(x, y).coordinateX, get_graph_node(x,y).coordinateY);
}

Entity* normal_student_new(Vector2D position, int gridX, int gridY) 
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/BeWitched!FemaleStudentWalk.png",
		32,
		32,
		4,
		0);
	ent->think = normal_student_think;
	ent->drawOffset = vector2d(16, 16);
	ent->speed = .5;
	ent->startFrame = 0;
	vector2d_copy(ent->position, position);
	ent->endFrame = 4;
	ent->currentGridPositionX = gridX;
	ent->currentGridPositionY = gridY;
	ent->targetGridPositionX = gridX;
	ent->targetGridPositionY = gridY;
	slog("h %i", ent->currentGridPositionX);
	//ent->targetGridPosition = get_next_tile(position.x, position.y, 1);

	//ent->targetGridPosition =  get_next_tile(position.x, position.y, 1);//ent->currentGridPosition->neighbours[1]; 
	//ent->currentGridPosition->neighbours[0];
	ent->index = 0;

	
	return ent;
}




void normal_student_think(Entity* self) {
	Vector2D dir;
	dir = self->direction;
	if (!self) return;
	dir = vector2d(0, 0);


	//vector2d_distance_between_less_than(graph_to_world_pos(self->currentGridPosition.x, self->currentGridPosition.y), );

	dir.y += 1;

	vector2d_set_magnitude(&dir, self->speed);
	vector2d_copy(self->velocity, dir);
}

void normal_student_remove_from_list(List *student_list, Entity* self) 
{
	gfc_list_delete_data(student_list, self);
	entity_free(self);
}