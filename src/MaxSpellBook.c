#include "simple_logger.h"
#include "Upgrades.h"
#include "TileMap.h"
#include "DynamicBody.h"
#include "Piper.h"
#include "UniversalData.h"

void MaxBook_update(Entity* self);

int MaxBook_collision(DynamicBody* self, List* collision);


Entity* Max_Spellbook_New(Vector2D position, Vector2D gridPosition)
{
	Entity* ent;
	ent = entity_new();

	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/SpellBagUpgrade.png",
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
	ent->body.touch = MaxBook_collision;
	ent->update = MaxBook_update;
	ent->tag = PlayerHazard;
	ent->shape.tag = Trigger;
	ent->shape.identifier = PlayerHazard;
	ent->counter = 5;

	vector2d_copy(ent->position, position);
	vector2d_copy(ent->currentGridPosition, gridPosition);
	level_add_entity(level_get_active_level(), ent);

	ent->timer = 0;
	ent->markedForDestruction = 0;//true
	return ent;
}


int MaxBook_collision(DynamicBody* self, List* collision)
{
	int i, selfIndex;
	Collision* other;

	Entity* selfEnt;
	selfEnt = self->entityAttached;

	if (GetPiperData()->currency < selfEnt->counter) {
		return 0;
	}

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
						piperDataPointer->maxSpellBooks += 1;
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

void MaxBook_destroy(Entity* self)
{

	level_remove_entity(self);
	entity_free(self);
}

void MaxBook_update(Entity* self)
{
	if (self->markedForDestruction == 1)
	{

			self->body.team = 3;
			self->timer = 0;
			self->markedForDestruction = 0;
			MaxBook_destroy(self);
		
	}
}

