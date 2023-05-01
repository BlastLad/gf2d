#include "simple_logger.h"
#include "TurretStudent.h"
#include "TileMap.h"
#include "DynamicBody.h"
#include "UniversalData.h"


void turret_student_think(Entity* self);

void turret_student_update(Entity* self);

int turret_on_collision(DynamicBody* self, List* collision);

int turret_on_world_collision(DynamicBody* self, List* collision);

void turret_student_destroy(Entity* self);

Entity* Turret_Student_New(Vector2D position, int gridPositionX, int gridPositionY)
{
	//slog("before x %f before y %f", gridPosition.x, gridPosition.y);	
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->index = 0;


	ent->sprite = gf2d_sprite_load_all("images/TurretStudent.png",
		16,
		16,
		4,
		0);
	ent->startFrame = 0;
	ent->endFrame = 4;

	

	ent->think = turret_student_think;
	ent->update = turret_student_update;
	ent->drawOffset = vector2d(8, 8);
	ent->speed = .5;

	//collion stuff
	ent->shape = gfc_shape_circle(0, 0, 5);
	ent->body.shape = &ent->shape;
	ent->body.inuse = true;
	ent->body.worldclip = 1;
	ent->body.ignore = false;
	ent->body.cliplayer = 1;
	ent->body.team = 1;
	ent->tag = Turret;
	ent->uniqueEntityTypeID = 17;

	vector2d_copy(ent->body.position, graph_to_world_pos(position.x, position.y));
	level_add_entity(level_get_active_level(), ent);
	//collision stuff end

	ent->body.touch = turret_on_collision;
	ent->body.worldtouch = turret_on_world_collision;

	ent->data = PathFinding(position.x, position.y, gridPositionX, gridPositionY);;




	if (get_path_list()->count > 0) {
		int indexer = 0;
		int maxSize = get_path_list()->count - 1;
		for (indexer = get_path_list()->count - 1; indexer >= 0; indexer--) {
			TileInfo* normalStudent;
			normalStudent = gfc_list_get_nth(get_path_list(), indexer);
			if (!normalStudent)continue;

			int coordX;
			int coordY;
			int thing;
			coordX = normalStudent->coordinates.x;
			coordY = normalStudent->coordinates.y;
			thing = normalStudent->tileFrame;
	
			int length = get_path_list()->count - 1;
			slog("here is length %i", length);

			if (indexer == maxSize)
			{
				vector2d_copy(ent->targetGridPosition, vector2d(coordX, coordY));
				slog("Next target x %i and y %i and thing %i", coordX, coordY, thing);
				gfc_list_delete_nth(get_path_list(), indexer);
			}

		}
	}


	vector2d_copy(ent->position, graph_to_world_pos(position.x, position.y));
	vector2d_copy(ent->currentGridPosition, vector2d(position.x, position.y));
	vector2d_copy(ent->originPosition, vector2d(gridPositionX, gridPositionY));


	ent->timer = 0;
	ent->markedForDestruction = 0;//false

	return ent;
}

int turret_on_collision(DynamicBody* self, List* collision) {

}

int turret_on_world_collision(DynamicBody* self, List* collision) {

}

void turret_student_update(Entity* self)
{
	//slog("Marked");



	//if (vector2d_distance_between_less_than(self->position, playerPos->position, 69.0)) {
	//	self->speed = 0.5f;
	//}
	//else {
	//	self->speed = 0.0f;
	//}


	if (self->currentGridPosition.y >= 11.0 && self->markedForDestruction == 0) {
		//despawn or mark for despawen?
		self->markedForDestruction = 1;
		//add points or smth
		//Nervous_student_destroy(self);
	}

	if (self->uniqueEntityTypeID == 17) 
	{

		self->timer += 0.1;

		if (self->timer > 10.0) {
			TileInfo* nextTile;

			

			
			self->timer = 0;
			self->uniqueEntityTypeID = 3;
		}

		//normal_student_destroy(self);
	}
}

void turret_student_think(Entity* self) {
	Vector2D dir;
	dir = self->direction;
	if (!self) return;
	dir = vector2d(0, 0);

	float mag;
	int stopMoving = 0;



	mag = vector2d_magnitude_between(graph_to_world_pos(self->targetGridPosition.x, self->targetGridPosition.y),
		graph_to_world_pos(self->currentGridPosition.x, self->currentGridPosition.y));
	

	if (vector2d_distance_between_less_than((graph_to_world_pos(self->targetGridPosition.x, self->targetGridPosition.y)), //1,1
		self->position, 2.0))  //1, 2
	{
		Vector2D oldTarget;// 1, 2
		vector2d_copy(oldTarget, self->targetGridPosition);// 1, 2

		if (get_path_list()->count > 0) 
		{
			int indexer = 0;
			for (indexer = get_path_list()->count - 1; indexer >= 0; indexer--) {
				TileInfo* normalStudent;
				normalStudent = gfc_list_get_nth(get_path_list(), indexer);
				if (!normalStudent)continue;

				int coordX;
				int coordY;
				int thing;
				coordX = normalStudent->coordinates.x;
				coordY = normalStudent->coordinates.y;
				thing = normalStudent->tileFrame;
				slog("Next target after x %i and y %i and thing %i", coordX, coordY, thing);
				int length = get_path_list()->count - 1;
				slog("here is length %i", length);

		
				vector2d_copy(self->targetGridPosition, vector2d(coordX, coordY));	
				gfc_list_delete_nth(get_path_list(), indexer);
				break;

			}
			vector2d_copy(self->currentGridPosition, oldTarget);
		}
		else {		
			stopMoving = 1;
		}
			
		

	}
	
	vector2d_sub(dir, graph_to_world_pos(self->targetGridPosition.x, self->targetGridPosition.y),
		graph_to_world_pos(self->currentGridPosition.x, self->currentGridPosition.y));

	slog("next is x %i y %i", dir.x, dir.y);

	//dir.y += 1;
	if (stopMoving == 1) {
		dir = vector2d(0, 0);
	}

	vector2d_normalize(&dir);
	vector2d_set_magnitude(&dir, self->speed);
	vector2d_copy(self->body.velocity, dir);
	vector2d_copy(self->velocity, dir);

	vector2d_copy(self->position, self->body.position);

	//vector2d_copy(pos, dir);*/


}

void turret_student_destroy(Entity* self) {
	level_remove_entity(self);
	entity_free(self);
}

void turret_student_remove_from_list(List* student_list, Entity* self)
{
	gfc_list_delete_data(student_list, self);
	level_remove_entity(self);
	entity_free(self);
}