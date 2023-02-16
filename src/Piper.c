#include "simple_logger.h"
#include "Piper.h"



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
	ent->drawOffset = vector2d(8, 8);
	ent->shape = gfc_shape_circle(0, 0, 5);
	ent->think = piper_think;
	vector2d_copy(ent->position, spawnPosition);
	ent->speed = 4;
	return ent;


}

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
		self->startFrame = 15;
		self->endFrame = 18;
	}
	if (keys[SDL_SCANCODE_D]) {
		dir.x += 1;
		self->startFrame = 5;
		self->endFrame = 8;
	}
	

	vector2d_set_magnitude(&dir, self->speed);
	vector2d_copy(self->velocity, dir);

}