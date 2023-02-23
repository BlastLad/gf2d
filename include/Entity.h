#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "gfc_shape.h"
#include "Body.h"


enum CollisonLayers
{
	AllFurniture = 1,//This team compri
	SolidFurniture = 2,
	CollideWithProjectilesAndStudent = 3,
	CollideWithProjectilesAndStudentAndPlayer = 4
};

enum EntityTags
{
	Student = 1,
	Player = 2,
	Furniture = 3,
	Projectile = 4
};
/**
*@brief the structure of an entity
*/
typedef struct Entity_S {
	Bool _inuse;
	Sprite *sprite;
	float frame;

	float startFrame;
	float endFrame;

	Shape shape;//the collision shape that will be used for colliders
	Body body;
	int layer;//or team
	int clips;//if false skip collisions


	int index;

	struct Entity_S* parent;
	enum EntityTags tag;

	Vector2D drawOffset;
	Vector2D position;
	Vector2D velocity;
	Vector2D direction;
	Vector2D currentGridPosition;
	Vector2D targetGridPosition;

	int markedForDestruction;

	float speed;

	void (*think)(struct Entity_S* self);
	void (*update)(struct Entity_S* self);
	void* data;//unique data

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


Shape entity_get_shape(Entity* ent);

Shape entity_get_shape_after_move(Entity* ent);

//@brief call all the think functions for the entities, if they have one
void entity_think_all();

#endif

