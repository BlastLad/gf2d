#include "simple_logger.h"
#include "Rugby.h"
#include "TileMap.h"
#include "DynamicBody.h"

void rugby_think(Entity* self);

void rugby_update(Entity* self);

int rugby_on_collision(DynamicBody* self, List* collision);

Entity* Rugby_New(Vector2D position, Vector2D gridPosition) 
{
	//slog("before x %f before y %f", gridPosition.x, gridPosition.y);	
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/Rugby.png",
		32,
		32,
		4,
		0);
	ent->think = rugby_think;
	ent->update = rugby_update;
	ent->drawOffset = vector2d(16, 16);
	ent->speed = 0.5;
	//collion stuff
	ent->shape = gfc_shape_circle(0, 0, 10);
	ent->body.inuse = true;
	ent->body.shape = &ent->shape;
	ent->body.worldclip = 1;
	ent->body.ignore = false;
	ent->body.cliplayer = 1, 2;
	ent->body.team = 3;
	ent->body.touch = rugby_on_collision;
	ent->body.entityAttached = ent;
	//ent->body.worldclip = rugby_on_collision;
	ent->tag = Furniture;
	vector2d_copy(ent->body.position, position);
	//gf2d_space_add_static_shape(level_get_active_level()->space, *ent->body.shape);

	level_add_entity(level_get_active_level(), ent);
	//collision stuff end

	ent->startFrame = 12;
	vector2d_copy(ent->position, position);
	ent->endFrame = 16;
	vector2d_copy(ent->currentGridPosition, gridPosition);
	vector2d_copy(ent->targetGridPosition, gridPosition);


	ent->index = 0;
	ent->markedForDestruction = 0;//false

	return ent;
}

int rugby_on_collision(DynamicBody* self, List* collision) {

	int i, selfIndex;
	Collision* other;


	for (i = 0; i < collision->count; i++)
	{
		other = (Collision*)gfc_list_get_nth(collision, i);
		if (!other) continue;
		
		//slog("Student collided %i", self->entityAttached->tag);
		if (other->collisionTag == Player && self->entityAttached->markedForDestruction == 0) {//collider with player
			self->entityAttached->body.team = 2;
			self->entityAttached->markedForDestruction = 1;
			//slog("Student collided %i", self->entityAttached->body.cliplayer);
			return 1;
		}
	}
	return 0;
}

void Rugby_remove_from_list(List* rugby_list, Entity* self) 
{

}

void rugby_update(Entity* self)
{

}



void rugby_think(Entity* self) {


	Vector2D dir;
	dir = self->direction;
	dir = vector2d(0, 0);

	if (vector2d_magnitude_compare(dir, 0) > 0) {
		vector2d_set_magnitude(&dir, self->speed);
		vector2d_copy(self->velocity, dir);
	}
	else
	{
		//slog("IN ELSE %f %f", self->body.position.x, self->body.position.y);
		vector2d_clear(self->body.velocity);
	}

	vector2d_normalize(&dir);
	vector2d_copy(self->body.velocity, dir);

	vector2d_copy(self->position, self->body.position);
}