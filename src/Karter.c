#include "simple_logger.h"
#include "Karter.h"
#include "TileMap.h"
#include "DynamicBody.h"
#include "AudioManager.h"

void karter_think(Entity* self);

void karter_update(Entity* self);

int karter_on_collision(DynamicBody* self, List* collision);

Entity* karter_new(Vector2D position, int gridPositionX, int gridPositionY, int targetGridX, int targetGridY) 
{
	//slog("before x %f before y %f", gridPosition.x, gridPosition.y);	
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/Karter.png",
		34,
		32,
		4,
		0);
	ent->think = karter_think;
	ent->update = karter_update;
	ent->body.touch = karter_on_collision;
	ent->drawOffset = vector2d(16, 16);
	ent->speed = .5;

	//collion stuff
	ent->shape = gfc_shape_circle(0, 0, 5);
	ent->shape.tag = Trigger;
	ent->shape.identifier = Furniture;

	ent->body.shape = &ent->shape;
	ent->body.inuse = true;
	ent->body.worldclip = 1;
	ent->body.ignore = false;
	ent->body.cliplayer = 1;
	ent->body.team = 3;
	ent->body.entityAttached = ent;
	ent->tag = Furniture;
	vector2d_copy(ent->body.position, position);
	level_add_entity(level_get_active_level(), ent);
	//collision stuff end
	ent->typeNum = 3;
	ent->startFrame = 0;
	vector2d_copy(ent->position, position);
	ent->endFrame = 4;
	vector2d_copy(ent->currentGridPosition, vector2d(gridPositionX, gridPositionY));
	vector2d_copy(ent->targetGridPosition, vector2d(targetGridX, targetGridY));

	ent->index = 0;
	ent->markedForDestruction = 0;//false

	return ent;
}

void karter_think(Entity* self) 
{

}

int karter_on_collision(DynamicBody* self, List* collision) 
{
	
}

void karter_update(Entity* self) 
{
	if (self->markedForDestruction == 1)
	{
		Vector2D dir;
		dir = self->direction;
		dir = vector2d(0, 0);

		vector2d_normalize(&dir);
		vector2d_set_magnitude(&dir, self->speed);
		vector2d_copy(self->body.velocity, dir);
		vector2d_copy(self->velocity, dir);

		vector2d_copy(self->position, self->body.position);
		self->body.team = 2;
		self->startFrame = 4;
		self->endFrame = 6;

		self->timer += 0.1;
		if (self->timer >= 128.0) {


			self->body.team = 3;
			self->timer = 0;
			self->startFrame = 0;
			self->endFrame = 4;
			self->markedForDestruction = 0;
			play_sound("audio/MagicReactive.mp3", 3);

		}
	}
	else {
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
			Vector2D oldTarget;// 1, 2
			vector2d_copy(oldTarget, self->targetGridPosition);// 1, 2
			vector2d_copy(self->targetGridPosition, self->currentGridPosition);
			vector2d_copy(self->currentGridPosition, oldTarget);

		}

		vector2d_sub(dir, graph_to_world_pos(self->targetGridPosition.x, self->targetGridPosition.y),
			graph_to_world_pos(self->currentGridPosition.x, self->currentGridPosition.y));

		//dir.y += 1;
		vector2d_normalize(&dir);
		vector2d_set_magnitude(&dir, self->speed);
		vector2d_copy(self->body.velocity, dir);
		vector2d_copy(self->velocity, dir);

		vector2d_copy(self->position, self->body.position);
	}
}