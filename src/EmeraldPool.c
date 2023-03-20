#include "simple_logger.h"
#include "Upgrades.h"
#include "TileMap.h"
#include "DynamicBody.h"
#include "Piper.h"
#include "UniversalData.h"
void Emerald_update(Entity* self);
void Emerald_think(Entity* self);
int Emerald_collision(DynamicBody* self, List* collision);


Entity* Emerald_New(Vector2D position, Vector2D gridPosition, int index)
{
	Entity* ent;
	ent = entity_new();

	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/Emerald.png",
		32,
		32,
		1,
		0);

	ent->shape = gfc_shape_circle(gridPosition.x * 32, gridPosition.y * 32, 10);

	ent->startFrame = 0;
	ent->endFrame = 1;
	ent->drawOffset = vector2d(16, 16);

	ent->shape = gfc_shape_circle(gridPosition.x * 32, gridPosition.y * 32, 10);
	ent->body.shape = &ent->shape;
	ent->body.inuse = false;
	ent->body.worldclip = 0;
	ent->body.cliplayer = 1;
	ent->body.team = 2;
	ent->body.ignore = false;
	ent->body.entityAttached = ent;
	ent->body.touch = Emerald_collision;
	ent->update = Emerald_update;
	ent->think = Emerald_think;
	ent->tag = PlayerHazard;
	ent->shape.tag = Trigger;
	ent->shape.identifier = PlayerHazard;
	ent->index = index;
	ent->uniqueEntityTypeID = -1;

	vector2d_copy(ent->position, graph_to_world_pos(gridPosition.x, gridPosition.y));
	vector2d_copy(ent->currentGridPosition, gridPosition);
	level_add_entity(level_get_active_level(), ent);
	ent->timer = 0;
	ent->markedForDestruction = 0;//true
	SetEmeraldPool(ent);
	slog("OK");

	return ent;
}


int Emerald_collision(DynamicBody* self, List* collision)
{
	
	return 0;
}

void Emerald_think(Entity* self)
{
	if (self->markedForDestruction == 0) {
		if (get_student_list()->count > 0) {
			int indexer = 0;
			for (indexer = get_student_list()->count - 1; indexer >= 0; indexer--) {
				Entity* normalStudent;
				normalStudent = gfc_list_get_nth(get_student_list(), indexer);
				if (!normalStudent)continue;
				if (normalStudent->markedForDestruction == 0)
				{
					if (vector2d_distance_between_less_than(self->position, normalStudent->position, 10)) {

						normalStudent->uniqueEntityTypeID = 4;//also deal damage
						self->markedForDestruction = 1;
					}
					//normal_student_remove_from_list(Students, normalStudent);
				}
			}
		}
	}
}

void Emerald_destroy(Entity* self)
{
	slog("HIT");
	resetEmeraldPoolStatus();
	level_remove_entity(self);
	entity_free(self);
}

void Emerald_update(Entity* self)
{
	if (self->markedForDestruction == 1)
	{


		self->body.team = 3;
		self->timer = 0;
		self->markedForDestruction = 0;
		Emerald_destroy(self);

	}
}
