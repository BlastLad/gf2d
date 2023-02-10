#include "simple_logger.h"
#include "Entity.h"

typedef struct 
{
	Uint32 entity_max;
	Entity *entity_list;
}EntityManager;

static EntityManager entity_manager = { 0 };

void entity_manager_close() 
{
	entity_free_all();
	if (entity_manager.entity_list)
	{
		free(entity_manager.entity_list);
	}
	slog("entity system closed");
	//entity_free_all();

}

void entity_manager_init(Uint32 max) 
{
	if (max <= 0) 
	{
		slog("cannot inialize entity system: zero entities specified!");
		return;
	}
	entity_manager.entity_list = gfc_allocate_array(sizeof(Entity), max);
	if (!entity_manager.entity_list) {
		slog("failed to initialize entity system!");
		return;
	}
	entity_manager.entity_max = max;
	atexit(entity_manager_close);//like an on disable but for the whole prog
	slog("entity system initialized");
}

void entity_free_all() {
	int i;
	for (i = 0; i < entity_manager.entity_max; i++) {
		if (!entity_manager.entity_list[i]._inuse)continue;
		entity_free(&entity_manager.entity_list[i]);
	}
}

Entity *entity_new() {//because we return a pointer to the new element of the list
	int i;
	for (i = 0; i < entity_manager.entity_max; i++) {
		if (entity_manager.entity_list[i]._inuse)continue;
		entity_manager.entity_list[i]._inuse = 1;//true
		//we could also initalize other values? try position maybe
		return &entity_manager.entity_list[i];

	}
	return NULL;
}


void entity_free(Entity *ent) {//because we take in the pointer to the element
	if (!ent) {
		slog("no entity provided");
		return;
	}
	if (ent->sprite)gf2d_sprite_free(ent->sprite);//sprite is an allocated type
	memset(ent, 0, sizeof(Entity));
}

void entity_draw(Entity* ent) {
	if (!ent)return;
	if (ent->sprite) 
	{	
		if (ent->sprite)
		{
			if (ent->frame < ent->startFrame) ent->frame = ent->startFrame;
			if (ent->frame > ent->endFrame) ent->frame = ent->startFrame;
			gf2d_sprite_draw(ent->sprite, ent->position, NULL, NULL, NULL, NULL, NULL, (int)ent->frame);

		}
	}
}

void entity_draw_all() 
{
	int i;
	for (i = 0; i < entity_manager.entity_max; i++) {
		if (!entity_manager.entity_list[i]._inuse)continue;
		entity_draw(&entity_manager.entity_list[i]);
	}

}


void entity_update(Entity* ent) {
	if (!ent)return;
	ent->frame += 0.1;
	if (ent->frame >= 16)ent->frame = 0;
	vector2d_add(ent->position, ent->position, ent->velocity);
}

void entity_update_all() 
{
	int i;
	for (i = 0; i < entity_manager.entity_max; i++) {
		if (!entity_manager.entity_list[i]._inuse)continue;
		entity_update(&entity_manager.entity_list[i]);
	}

}

void entity_think(Entity* ent) {
	if (!ent)return;
	if(ent->think)ent->think(ent);//if the entity has a think method call it and pass in self
}

void entity_think_all() {
	int i;
	for (i = 0; i < entity_manager.entity_max; i++) {
		if (!entity_manager.entity_list[i]._inuse)continue;
		entity_think(&entity_manager.entity_list[i]);
	}
}

