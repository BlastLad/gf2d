#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "Collision.h"
#include "NormalStudent.h"
#include "DynamicBody.h"
#include "TileMap.h"
#include "AudioManager.h"
#include "Piper.h"


void lost_student_think(Entity* self);

void lost_student_update(Entity* self);

int lost_on_collision(DynamicBody* self, List* collision);

int lost_on_world_collision(DynamicBody* self, List* collision);

void lost_student_destroy(Entity* self);


Entity* lost_student_new(Vector2D position, int gridPositionX, int gridPositionY, int targetGridX, int targetGridY, int index)
{
	//slog("before x %f before y %f", gridPosition.x, gridPosition.y);	
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;

	ent->index = index;
	if (index == 1) {
		ent->sprite = gf2d_sprite_load_all("images/MaleStudentLost.png",
			16,
			16,
			5,
			0);
		ent->startFrame = 9;
		ent->endFrame = 12;
	}
	else if (index == 0) {		
		ent->sprite = gf2d_sprite_load_all("images/LostStudentFemale.png",
			16,
			16,
			5,
			0);
		ent->startFrame = 0;
		ent->endFrame = 4;
	}

	ent->think = lost_student_think;
	ent->update = lost_student_update;
	ent->drawOffset = vector2d(8, 8);
	ent->speed = .5;
	if (GetHellFireData()->rushHour == 1)
		ent->speed = 1;

	//collion stuff
	ent->shape = gfc_shape_circle(0, 0, 5);
	ent->body.shape = &ent->shape;
	ent->body.inuse = true;
	ent->body.worldclip = 1;
	ent->body.ignore = false;
	ent->body.cliplayer = 2;
	ent->body.team = 2;
	ent->tag = Student;
	ent->uniqueEntityTypeID = 3;
	vector2d_copy(ent->body.position, position);
	level_add_entity(level_get_active_level(), ent);
	//collision stuff end

	ent->body.touch = lost_on_collision;
	ent->body.worldtouch = lost_on_world_collision;

	vector2d_copy(ent->position, position);
	vector2d_copy(ent->currentGridPosition, vector2d(gridPositionX, gridPositionY));
	vector2d_copy(ent->originPosition, vector2d(gridPositionX, gridPositionY));
	vector2d_copy(ent->targetGridPosition, vector2d(targetGridX, targetGridY));

	ent->timer = 0;
	ent->counter = 0;
	ent->markedForDestruction = 0;//false

	return ent;
}


void lost_student_update(Entity* self)
{
	//slog("Marked");


	if (self->currentGridPosition.y >= 11.0 && self->markedForDestruction == 0) {
		self->markedForDestruction = 1;
		if (GetPiperData()->currency < 21)
		{
			GetPiperData()->currency += 1;
		}
		//;ost_student_destroy(self);
	}
	else if (self->markedForDestruction == 1) {
		//lost_student_destroy(self);
	}
	else if (self->counter > 7) {
		if (self->currentGridPosition.y <= 1.0 && self->markedForDestruction == 0) {
			self->markedForDestruction = 1;
			if (GetPiperData()->currency < 21)
			{
				GetPiperData()->currency += 1;
			}
			//lost_student_destroy(self);
		}
	}

	if (self->uniqueEntityTypeID == 4) {

		self->timer += 0.1;

		float target = 50.0;
		if (GetHellFireData()->rushHour == 1)
			target = 30.0;

		if (self->timer > target) {
			self->timer = 0;
			self->uniqueEntityTypeID = 3;
		}

		//normal_student_destroy(self);
	}
}

int lost_on_world_collision(DynamicBody* self, List* collision)
{
	int i;
	Collision* other;

	for (i = 0; i < collision->count; i++)
	{
		other = (Collision*)gfc_list_get_nth(collision, i);
		if (!other) continue;
		if (other->shape.identifier == Furniture)
		{//collider with player
			if (self->entityAttached->uniqueEntityTypeID == 3)
				self->entityAttached->markedForDestruction = 1;
			return 1;
		}
	}

	return 0;
}

int lost_on_collision(DynamicBody* self, List* collision) {
	int i, selfIndex;
	Collision* other;


	for (i = 0; i < collision->count; i++)
	{
		other = (Collision*)gfc_list_get_nth(collision, i);
		if (!other) continue;
		//
		//slog("Student collided %i", self->entityAttached->tag);
		if (other->collisionTag == Furniture) {

			//slog("Student collided %i", other->collisionTag);
			if (self->entityAttached->uniqueEntityTypeID == 3) {
				self->entityAttached->markedForDestruction = 1;
				play_sound("audio/StudentEaten.mp3", 2);
			}
			return 1;
		}
	}

	return 0;
}

void lost_student_think(Entity* self) {
	Vector2D dir;
	dir = self->direction;
	if (!self) return;
	dir = vector2d(0, 0);

	float mag;

	mag = vector2d_magnitude_between(graph_to_world_pos(self->targetGridPosition.x, self->targetGridPosition.y),
		graph_to_world_pos(self->currentGridPosition.x, self->currentGridPosition.y));


	if (vector2d_distance_between_less_than((graph_to_world_pos(self->targetGridPosition.x, self->targetGridPosition.y)), //1,1
		self->position, 2.0))  //1, 2
	{
		if (self->counter == 7) 
		{
			self->counter++;
			get_next_previous_tile(self->targetGridPosition.x, self->targetGridPosition.y, self); //1, 3

		}
		else 
		{
			self->counter++;
			Vector2D oldTarget;// 1, 2
			vector2d_copy(oldTarget, self->targetGridPosition);// 1, 2
			get_next_carpet_tile(self->currentGridPosition.x, self->currentGridPosition.y, self); //1, 3
			vector2d_copy(self->currentGridPosition, oldTarget);
		}


	}

	vector2d_sub(dir, graph_to_world_pos(self->targetGridPosition.x, self->targetGridPosition.y),
		graph_to_world_pos(self->currentGridPosition.x, self->currentGridPosition.y));

	//dir.y += 1;
	vector2d_normalize(&dir);
	vector2d_set_magnitude(&dir, self->speed);
	vector2d_copy(self->body.velocity, dir);
	vector2d_copy(self->velocity, dir);

	vector2d_copy(self->position, self->body.position);

	//vector2d_copy(pos, dir);


}

void lost_student_destroy(Entity* self) {
	set_current_level_remainingStudents(get_current_level_remainingStudents() - 1);
	level_remove_entity(self);
	entity_free(self);
}

void lost_student_remove_from_list(List* student_list, Entity* self)
{
	gfc_list_delete_data(student_list, self);
	level_remove_entity(self);
	entity_free(self);
}