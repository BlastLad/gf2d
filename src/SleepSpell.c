#include "TileMap.h"
#include "simple_logger.h"
#include "DynamicBody.h"
#include "SleepSpell.h"
#include "AudioManager.h"

void sleep_spell_think(Entity* self);

void sleep_spell_update(Entity* self);

int sleep_spell_collision(DynamicBody* self, List* collision);

int sleep_spell_world_collision(DynamicBody* self, List* collision);

void sleep_spell_destroy(Entity* self);

Entity* sleep_spell_new(Vector2D spawnPosition, Entity* parent, Vector2D direction) {
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/BeWitched!Star.png",
		16,
		16,
		3,
		0);

	ent->parent = parent;
	ent->direction = direction;

	ent->update = sleep_spell_update;
	ent->drawOffset = vector2d(8, 8);
	ent->speed = 3;

	ent->currentGridPosition = spawnPosition;//this will be for timing distance 
	//collion stuff
	ent->shape = gfc_shape_circle(0, 0, 5);
	ent->body.shape = &ent->shape;
	ent->body.inuse = true;
	ent->body.worldclip = 1;
	ent->body.ignore = false;
	ent->body.cliplayer = 1;//same as player
	ent->body.team = 1;//same as player
	ent->tag = Projectile;
	ent->body.entityAttached = ent;
	vector2d_copy(ent->body.position, spawnPosition);
	level_add_entity(level_get_active_level(), ent);
	//collision stuff end
	ent->body.worldtouch = sleep_spell_world_collision;
	ent->body.touch = sleep_spell_collision;

	ent->startFrame = 0;
	vector2d_copy(ent->position, spawnPosition);
	ent->endFrame = 4;

	ent->index = 0;
	ent->markedForDestruction = 0;//false

	return ent;
}

int sleep_spell_world_collision(DynamicBody* self, List* collision) 
{
	
	int i;
	Collision* other;

	for (i = 0; i < collision->count; i++)
	{
		other = (Collision*)gfc_list_get_nth(collision, i);
		if (!other) continue;
		
		if (other->collisionTag == Furniture) {//collider with player

			Entity* ent;
			ent = other->ent;
	
			if (ent) {
				slog("Student collided wrold %i", other->collisionTag);
				if (ent)
				{
					ent->markedForDestruction = 1;
					self->entityAttached->markedForDestruction = 1;
					ent->startFrame = 0;
					ent->endFrame = 2;
					play_sound("audio/GamingSouns.mp3", 1);
					return 1;
				}
			}
			//slog("Student collided %i", self->entityAttached->body.cliplayer);
		
		}

		if (self->blocked) 
		{
			self->entityAttached->markedForDestruction = 1;
		}
	}

	return 1;
}

int sleep_spell_collision(DynamicBody* self, List* collision)
{
	int i;
	Collision* other;

	
	for (i = 0; i < collision->count; i++)
	{
		other = (Collision*)gfc_list_get_nth(collision, i);
		if (!other) continue;
		
		if (other->collisionTag == Furniture) {//collider with player
			
			Body* bod;
			bod = other->body;
		
			if (bod) {
				bod->team = 2;
				Entity* ent;
				ent = bod->entityAttached;
				if (ent) 
				{					
					ent->markedForDestruction = 1;
					if (ent->index == 0) {
						ent->timer = 0;
					}
					self->entityAttached->markedForDestruction = 1;

					ent->startFrame = 3;
					ent->endFrame = 7;
					play_sound("audio/GamingSouns.mp3", 1);				
				}
			}
			//slog("Student collided %i", self->entityAttached->body.cliplayer);
			return 1;
		}
	}
	return 0;
}


void sleep_spell_destroy(Entity* self) {
	level_remove_entity(self);
	entity_free(self);
}

void sleep_spell_update(Entity* self)
{
    if (!self)return;

	if (self->markedForDestruction == 1) {
		sleep_spell_destroy(self);
		return;
	}

	Vector2D dir;
	dir = self->direction;



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
	vector2d_set_magnitude(&dir, self->speed);
	vector2d_copy(self->body.velocity, dir);

	vector2d_copy(self->position, self->body.position);

}