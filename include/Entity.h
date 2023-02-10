#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"


/**
*@brief the structure of an entity
*/
typedef struct Entity_S {
	Bool _inuse;
	Sprite *sprite;
	float frame;

	float startFrame;
	float endFrame;

	Vector2D position;
	Vector2D velocity;
	Vector2D direction;

	float speed;

	void (*think)(struct Entity_S* self);

}Entity;

/**
*@brief initialize the internal manager for the entity system
*@note this will automatically queue up the close function for program exit
*@param max this is the maximum number of supported entities at a given time
*/
void entity_manager_init(Uint32 max);


/**
*@brief allocate an initialize a new entity
*@return NULL if there are no entities left, an empty entity otherwise
*/
Entity *entity_new();

/**
*@brief free a previously allocated entity
*@return null if there are no entities left, an empty entity otherwise
*/
void entity_free(Entity *ent);

//@brief free all allocated entities
void entity_free_all();

//@brief draw all active entities if they have graphics
void entity_draw_all();

//@brief update all active entities
void entity_update_all();


//@brief call all the think functions for the entities, if they have one
void entity_think_all();

#endif

