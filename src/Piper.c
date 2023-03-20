#include "simple_logger.h"
#include "Piper.h"
#include "TileMap.h"
#include "SleepSpell.h"
#include "MixingSpell.h"
#include "DynamicBody.h"
#include "ShadowClone.h"
#include "UniversalData.h"


void piper_think(Entity* self);

void piper_update(Entity* self);

int piper_on_static_collision(DynamicBody* self, List* collision);





static PiperData piperData = {
	false, //sleepUpgrade
	false,
	3,
	3,
	0,
	1,
	1,
	0,
	0,
	0,
	0
};

PiperData* GetPiperData() {
	return &piperData;
}

void SetTagsPlayer(Entity* self) 
{
	self->tag = Player;
	self->shape.tag = Solid;
	self->shape.identifier = Player;
}

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
	ent->update = piper_update;
	ent->body.inuse = true;
	ent->body.entityAttached = ent;
	ent->drawOffset = vector2d(8, 8);
	ent->shape = gfc_shape_circle(0, 0, 5);
	ent->uniqueEntityTypeID = 1;
	SetTagsPlayer(ent);
	//ent->tag = Player;
	//ent->shape.tag = Player;
	ent->body.shape = &ent->shape;
	ent->body.worldclip = 1;
	ent->body.ignore = false;
	ent->body.cliplayer = 1;
	ent->body.team = 1;

	vector2d_copy(ent->body.position, spawnPosition);
	level_add_entity(level_get_active_level(), ent);

	ent->body.worldtouch = piper_on_static_collision;

	vector2d_copy(ent->position, spawnPosition);
	ent->speed = 2;
	ent->markedForDestruction = 0;
	ent->data = (void*)&piperData;
	ent->counter = 0;
	return ent;


}

static int spacebarDown = 0;
static int qKeyDown = 0;
static int lShift = 0;
static int shadowSpawned = 0;

int piper_on_static_collision(DynamicBody* self, List* collision) 
{

}

void piper_update(Entity* self)
{
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

	if (self->speed > 2) {
		piperData.timeSpedUp += 0.1;
		if (piperData.timeSpedUp > 30.0) {
			self->speed = 2;
			piperData.timeSpedUp = 0.0;
		}
	}


	if (self->counter >= 1) 
	{
		self->counter += 1;

		if (self->counter > 900) 
		{
			self->counter = 0;
			shadowSpawned = 0;
		}
	}
}

void SleepSpellCast(Vector2D direction, Entity* ent) 
{
	if (direction.x == 0 && direction.y == 0)
		direction.y += 1;
	sleep_spell_new(ent->position, ent, direction);

	if (piperData.sleepUpgrade == true) 
	{
		vector2d_negate(direction, direction);
		sleep_spell_new(ent->position, ent, direction);
	}
}

void MixingSpellCast(Vector2D direction, Entity* ent) 
{
	if (piperData.mixAbility == 1) {
		if (direction.x == 0 && direction.y == 0)
			direction.y += 1;
		mixing_spell_new(ent->position, ent, direction);
	}
}

void ShadowClone(Entity* self) 
{
	if (piperData.shadowClone) {
		shadow_clone_entity_new(self->position);
		self->counter = 1;
		shadowSpawned = 1;
	}
}



void piper_think(Entity* self) {

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
		self->endFrame = 19;
	}
	if (keys[SDL_SCANCODE_D]) {
		dir.x += 1;
		self->startFrame = 5;
		self->endFrame = 8;
	}
	
	if (keys[SDL_SCANCODE_SPACE] && spacebarDown == 0) 
	{
		spacebarDown = 1;
		SleepSpellCast(dir, self);	

	}
	else if (!keys[SDL_SCANCODE_SPACE]) 
	{
		spacebarDown = 0;
	}

	if (keys[SDL_SCANCODE_Q] && qKeyDown == 0)
	{
		qKeyDown = 1;
		MixingSpellCast(dir, self);

	}
	else if (!keys[SDL_SCANCODE_Q])
	{
		qKeyDown = 0;
	}

	if (keys[SDL_SCANCODE_LSHIFT]) {
		lShift = 1;
		if (shadowSpawned == 0) {
			ShadowClone(self);
		}
	}
	else if (!keys[SDL_SCANCODE_LSHIFT]) {
		lShift = 0;
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