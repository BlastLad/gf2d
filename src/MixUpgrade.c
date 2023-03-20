#include "simple_logger.h"
#include "Upgrades.h"
#include "TileMap.h"
#include "DynamicBody.h"
#include "Piper.h"

void MixUpgrade_update(Entity* self);

int MixUpgrade_collision(DynamicBody* self, List* collision);


Entity* Mix_Upgrade_New(Vector2D position, Vector2D gridPosition)
{
	Entity* ent;
	ent = entity_new();

	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/MixUpgrade.png",
		32,
		32,
		1,
		0);

	ent->shape = gfc_shape_rect(gridPosition.x * 32, gridPosition.y * 32, 32, 32);

	ent->startFrame = 0;
	ent->endFrame = 0;
	ent->drawOffset = vector2d(16, 16);

	ent->shape = gfc_shape_rect(gridPosition.x * 32, gridPosition.y * 32, 32, 32);
	ent->body.shape = &ent->shape;
	ent->body.inuse = false;
	ent->body.worldclip = 0;
	ent->body.cliplayer = 1;
	ent->body.team = 2;
	ent->body.ignore = false;
	ent->body.entityAttached = ent;
	ent->body.touch = MixUpgrade_collision;
	ent->update = MixUpgrade_update;
	ent->tag = PlayerHazard;
	ent->shape.tag = Trigger;
	ent->shape.identifier = PlayerHazard;

	vector2d_copy(ent->position, position);
	vector2d_copy(ent->currentGridPosition, gridPosition);
	level_add_entity(level_get_active_level(), ent);

	ent->timer = 0;
	ent->markedForDestruction = 0;//true
	return ent;
}


int MixUpgrade_collision(DynamicBody* self, List* collision)
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
					//cast as piper data
					if (ent->uniqueEntityTypeID == 1 && self->entityAttached->markedForDestruction == 0) {
						PiperData* piperDataPointer;
						piperDataPointer = (struct PiperData*)ent->data;
						piperDataPointer->mixingUpgrade = 1;						
						self->entityAttached->markedForDestruction = 1;					
						return 1;
					}
				}
			}

		}
	}

	return 0;
}

void MixUpgrade_destroy(Entity* self)
{
	slog("HIT");
	level_remove_entity(self);
	entity_free(self);
}

void MixUpgrade_update(Entity* self)
{
	if (self->markedForDestruction == 1)
	{


			self->body.team = 3;
			self->timer = 0;
			self->markedForDestruction = 0;
			MixUpgrade_destroy(self);
		
	}
}

