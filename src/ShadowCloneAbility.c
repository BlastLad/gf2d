#include "simple_logger.h"
#include "Upgrades.h"
#include "TileMap.h"
#include "DynamicBody.h"
#include "Piper.h"
#include "UniversalData.h"

void Shadow_Clone_PickUp_update(Entity* self);

int Shadow_Clone_PickUp_collision(DynamicBody* self, List* collision);


Entity* Shadow_Clone_PickUp_New(Vector2D position, Vector2D gridPosition)
{
	Entity* ent;
	ent = entity_new();

	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/ShadowCloneBase.png",
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
	ent->body.touch = Shadow_Clone_PickUp_collision;
	ent->update = Shadow_Clone_PickUp_update;
	ent->tag = PlayerHazard;
	ent->shape.tag = Trigger;
	ent->shape.identifier = PlayerHazard;
	ent->counter = 10;

	vector2d_copy(ent->position, position);
	vector2d_copy(ent->currentGridPosition, gridPosition);
	level_add_entity(level_get_active_level(), ent);

	ent->timer = 0;
	ent->markedForDestruction = 0;//true
	return ent;
}


int Shadow_Clone_PickUp_collision(DynamicBody* self, List* collision)
{
	int i, selfIndex;
	Collision* other;


	for (i = 0; i < collision->count; i++)
	{
		other = (Collision*)gfc_list_get_nth(collision, i);

		Entity* selfEnt;
		selfEnt = self->entityAttached;

		if (GetPiperData()->currency < selfEnt->counter) {
			return 0;
		}

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
						piperDataPointer->shadowClone = 1;
						self->entityAttached->markedForDestruction = 1;
						int value = selfEnt->counter;
						OnPowerUpCollect(value);
						return 1;
					}
				}
			}

		}
	}

	return 0;
}

void Shadow_Clone_PickUp_destroy(Entity* self)
{

	level_remove_entity(self);
	entity_free(self);
}

void Shadow_Clone_PickUp_update(Entity* self)
{
	if (self->markedForDestruction == 1)
	{


			self->body.team = 3;
			self->timer = 0;
			self->markedForDestruction = 0;
			Shadow_Clone_PickUp_destroy(self);		
	}
}

