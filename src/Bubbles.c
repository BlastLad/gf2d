#include "simple_logger.h"
#include "Bubbles.h"
#include "TileMap.h"
#include "DynamicBody.h"


void bubbles_think(Entity* self);

void bubbles_update(Entity* self);

int bubbles_on_collision(DynamicBody* self, List* collision);


void SetTagsFurnitureCollider(Entity* self)
{
	self->tag = Furniture;
	self->shape.tag = Solid;
	self->shape.identifier = Furniture;
}

void SetTagsNonFurnitureCollider(Entity* self)
{
	self->tag = NonsolidFurniture;
	self->shape.tag = Trigger;
}

typedef struct
{
	Shape boxShape;
	Entity* parent;
}BubbleEatBox;


Entity* Bubbles_New(Vector2D position, Vector2D gridPosition, Vector2D attackTargetGridPosition) 
{
	//entity init
	Entity* ent;
	BubbleEatBox bubbleEatBox;
	ent = entity_new();
	bubbleEatBox.boxShape = gfc_shape_rect(attackTargetGridPosition.x * 32, attackTargetGridPosition.y * 32, 32, 32);
	bubbleEatBox.boxShape.tag = Trigger;
	bubbleEatBox.boxShape.identifier = Furniture;
	bubbleEatBox.parent = ent;
	
	slog("1");
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/BubblesWithHandles.png",
		32,
		32,
		4,
		0);

	ent->startFrame = 3;
	ent->endFrame = 7;
	slog("2");
	//entity method init
	ent->think = bubbles_think;
	ent->update = bubbles_update;

	ent->drawOffset = vector2d(16, 16);
	ent->speed = 0.5;

	//collion stuff

	slog("3");
	//collion stuff
	//Shape shape;
	//shape = gfc_shape_rect(2 * 32, 2 * 32, 32, 32);
	ent->shape = gfc_shape_rect(gridPosition.x * 32, gridPosition.y * 32, 32, 32);
	ent->body.inuse = false;
	ent->body.entityAttached = ent;
	SetTagsFurnitureCollider(ent);

	gf2d_space_add_static_shape(level_get_active_level()->space, bubbleEatBox.boxShape);
	gf2d_space_add_static_shape(level_get_active_level()->space, ent->shape);

	ent->data = (void*)&bubbleEatBox;

	//vector2d_copy(ent->body.position, position);
	//level_add_entity(level_get_active_level(), ent);
	//collision stuff end


	vector2d_copy(ent->position, position);
	vector2d_copy(ent->currentGridPosition, gridPosition);
	vector2d_copy(ent->targetGridPosition, gridPosition);

	ent->timer = 0;
	ent->markedForDestruction = 0;//false
	return ent;
}

void bubbles_think(Entity* self) {

}

void bubbles_update(Entity* self) 
{

}