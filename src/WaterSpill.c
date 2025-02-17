#include "simple_logger.h"
#include "WaterSpill.h"
#include "TileMap.h"
#include "DynamicBody.h"

void water_spill_update(Entity* self);

int spill_on_collision(DynamicBody* self, List* collision);

void SetTagsHazard(Entity* self)
{
	self->tag = PlayerHazard;
	self->shape.tag = Trigger;
	self->shape.identifier = PlayerHazard;
}

Entity* Water_Spill_New(Vector2D position, Vector2D gridPosition) 
{
	Entity* ent;
	ent = entity_new();

	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/Water.png",
		32,
		32,
		2,
		0);

	ent->shape = gfc_shape_rect(gridPosition.x * 32, gridPosition.y * 32, 32, 32);

	ent->startFrame = 1;
	ent->endFrame = 2;
	ent->drawOffset = vector2d(16, 16);

	ent->shape = gfc_shape_rect(gridPosition.x * 32, gridPosition.y * 32, 32, 32);
	ent->body.shape = &ent->shape;
	ent->body.inuse = false;
	ent->body.worldclip = 0;
	ent->body.cliplayer = 1;
	ent->body.team = 2;
	ent->body.ignore = false;
	ent->body.entityAttached = ent;
	ent->body.touch = spill_on_collision;
	ent->update = water_spill_update;
	SetTagsHazard(ent);

	vector2d_copy(ent->position, position);
	vector2d_copy(ent->currentGridPosition, gridPosition);
	level_add_entity(level_get_active_level(), ent);

	ent->timer = 0;
	ent->markedForDestruction = 1;//true
	return ent;
}


int spill_on_collision(DynamicBody* self, List* collision) 
{
	int i, selfIndex;
	Collision* other;


	for (i = 0; i < collision->count; i++)
	{
		other = (Collision*)gfc_list_get_nth(collision, i);
		if (!other) continue;
		//
		//slog("Student collided %i", self->entityAttached->tag);
		if (other->collisionTag == Player) {

			Body* body;
			body = other->body;
			if (body) 
			{
				Entity* ent;
				ent = body->entityAttached;
				if (ent) 
				{
					ent->markedForDestruction = 1;
					ent->startFrame = 4; 
					ent->endFrame = 10;
					self->entityAttached->timer = 32.0;
					return 1;
				}
			}

		}
	}

	return 0;
}

void water_spill_destroy(Entity* self) 
{
	
	level_remove_entity(self);
	entity_free(self);
}

void water_spill_update(Entity* self) 
{
	if (self->markedForDestruction == 1)
	{
		self->timer += 0.1;
		if (self->timer >= 32.0) {


			self->body.team = 3;
			self->timer = 0;
			self->markedForDestruction = 0;
			water_spill_destroy(self);
		}
	}
}

