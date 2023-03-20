#include "simple_logger.h"
#include "ShadowClone.h"
#include "TileMap.h"
#include "SleepSpell.h"
#include "MixingSpell.h"
#include "DynamicBody.h"
#include "Piper.h"



void shadow_clone_think(Entity* self);

void shadow_clone_update(Entity* self);

int shadow_clone_on_static_collision(DynamicBody* self, List* collision);


void SetTagsShadow(Entity* self)
{
	self->tag = Player;
	self->shape.tag = Solid;
	self->shape.identifier = Player;
}

Entity* shadow_clone_entity_new(Vector2D spawnPosition)
{
	Entity* ent;
	ent = entity_new();
	if (!ent) return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/ShadowClone.png",
		16,
		16,
		4,
		0);
	ent->think = shadow_clone_think;
	ent->update = shadow_clone_update;
	ent->body.inuse = true;
	ent->body.entityAttached = ent;
	ent->drawOffset = vector2d(8, 8);
	ent->shape = gfc_shape_circle(0, 0, 5);
	SetTagsShadow(ent);
	//ent->tag = Player;
	//ent->shape.tag = Player;
	ent->body.shape = &ent->shape;
	ent->body.worldclip = 1;
	ent->body.ignore = false;
	ent->body.cliplayer = 1;
	ent->body.team = 1;

	vector2d_copy(ent->body.position, spawnPosition);
	level_add_entity(level_get_active_level(), ent);

	ent->body.worldtouch = shadow_clone_on_static_collision;

	vector2d_copy(ent->position, spawnPosition);
	ent->speed = 2;
	ent->markedForDestruction = 0;
	ent->counter = 0;
	ent->index = 0;
	return ent;


}

static int spacebarDown = 0;
static int qKeyDown = 0;

int shadow_clone_on_static_collision(DynamicBody* self, List* collision)
{

}

void shadow_clone_update(Entity* self)
{
	self->index += 1;

	if (self->index >= 600) 
	{
		level_remove_entity(self);
		entity_free(self);
	}


	if (self->markedForDestruction == 1)
	{
		self->timer += 0.1;
		if (self->timer >= 10.0) {

			self->startFrame = 0;
			self->endFrame = 0;
			self->timer = 0;
			self->markedForDestruction = 0;
		}
	}
}

void ShadowSleepSpellCast(Vector2D direction, Entity* ent)
{
	if (direction.x == 0 && direction.y == 0)
		direction.y += 1;
	sleep_spell_new(ent->position, ent, direction);

	if (GetPiperData()->sleepUpgrade) 
	{
		vector2d_negate(direction, direction);
		sleep_spell_new(ent->position, ent, direction);
	}
	/*if (piperData.sleepUpgrade == true)
	{
		vector2d_negate(direction, direction);
		sleep_spell_new(ent->position, ent, direction);
	}*/
}

void ShadowMixingSpellCast(Vector2D direction, Entity* ent)
{
	if (GetPiperData()->mixAbility) 
	{
		if (direction.x == 0 && direction.y == 0)
			direction.y += 1;
		mixing_spell_new(ent->position, ent, direction);
	}

	if (GetPiperData()->mixingUpgrade) {

	}

	/*if (piperData.mixingUpgrade == true)
	{
		vector2d_negate(direction, direction);
		mixing_spell_new(ent->position, ent, direction);
	}*/
}



void shadow_clone_think(Entity* self) {

	if (!self) return;
	Vector2D dir;
	dir = self->direction;
	dir = vector2d(0, 0);
	if (self->markedForDestruction == 1)
	{
		vector2d_clear(self->body.velocity);
		vector2d_normalize(&dir);
		vector2d_copy(self->body.velocity, dir);
		vector2d_copy(self->position, self->body.position);
		return;
	}
	Uint8* keys;
	keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
	self->startFrame = 0;
	self->endFrame = 0;
	if (keys[SDL_SCANCODE_W]) {
		dir.y += 1;
		self->startFrame = 0;
		self->endFrame = 3;
	}
	if (keys[SDL_SCANCODE_S]) {
		dir.y -= 1;
		self->startFrame = 10;
		self->endFrame = 13;
	}
	if (keys[SDL_SCANCODE_A]) {
		dir.x += 1;
		self->startFrame = 5;
		self->endFrame = 8;
	}
	if (keys[SDL_SCANCODE_D]) {
		dir.x -= 1;
	
		self->startFrame = 15;
		self->endFrame = 19;
	}

	if (keys[SDL_SCANCODE_SPACE] && spacebarDown == 0)
	{
		spacebarDown = 1;
		ShadowSleepSpellCast(dir, self);

	}
	else if (!keys[SDL_SCANCODE_SPACE])
	{
		spacebarDown = 0;
	}

	if (keys[SDL_SCANCODE_Q] && qKeyDown == 0)
	{
		qKeyDown = 1;
		ShadowMixingSpellCast(dir, self);

	}
	else if (!keys[SDL_SCANCODE_Q])
	{
		qKeyDown = 0;
	}


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