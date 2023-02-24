#include "simple_logger.h"
#include "Piper.h"
#include "TileMap.h"
#include "SleepSpell.h"


void piper_think(Entity* self);

Entity* piper_entity_new(Vector2D spawnPosition)
{
	Entity* ent;
	ent = entity_new();
	if (!ent) return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/BeWitched_PlayerWalk.png",
		16,
		16,
		4,
		0);	
	ent->think = piper_think;
	ent->body.inuse = true;
	ent->drawOffset = vector2d(8, 8);
	ent->shape = gfc_shape_circle(0, 0, 5);
	ent->body.shape = &ent->shape;
	ent->body.worldclip = 1;
	ent->body.ignore = false;
	ent->body.cliplayer = 1;
	ent->body.team = 1;
	ent->tag = Player;

	vector2d_copy(ent->body.position, spawnPosition);
	level_add_entity(level_get_active_level(), ent);

	vector2d_copy(ent->position, spawnPosition);
	ent->speed = 4;
	return ent;


}

static int spacebarDown = 0;

void piper_think(Entity* self) {

	if (!self) return;
	Vector2D dir;
	dir = self->direction;
	Uint8* keys;
	keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
	dir = vector2d(0, 0);
	self->startFrame = 0;
	self->endFrame = 0;
	if (keys[SDL_SCANCODE_W]) {
		dir.y -= 1;
		self->startFrame = 10;
		self->endFrame = 13;
	}
	if (keys[SDL_SCANCODE_S]) {
		dir.y += 1;
		self->startFrame = 0;
		self->endFrame = 3;
	}
	if (keys[SDL_SCANCODE_A]) {
		dir.x -= 1;
		self->startFrame = 5;
		self->endFrame = 18;
	}
	if (keys[SDL_SCANCODE_D]) {
		dir.x += 1;
		self->startFrame = 5;
		self->endFrame = 8;
	}
	
	if (keys[SDL_SCANCODE_SPACE] && spacebarDown == 0) 
	{
		spacebarDown = 1;
		if (dir.x == 0 && dir.y == 0)
			dir.y += 1;
		sleep_spell_new(self->position, self, dir);

	}
	else if (!keys[SDL_SCANCODE_SPACE]) 
	{
		spacebarDown = 0;
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
	vector2d_copy(self->body.velocity, dir);

    vector2d_copy(self->position, self->body.position);

}