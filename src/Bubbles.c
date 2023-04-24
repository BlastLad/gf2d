#include "simple_logger.h"
#include "Bubbles.h"
#include "TileMap.h"
#include "DynamicBody.h"
#include "UniversalData.h"
#include "Piper.h"
#include "Upgrades.h"
#include "AudioManager.h"
void bubbles_think(Entity* self);

void bubbles_update(Entity* self);

int bubbles_on_collision(DynamicBody* self, List* collision);


void SetTagsFurnitureCollider(Entity* self)
{
	self->tag = Furniture;
	self->shape.tag = Solid;
	self->shape.identifier = Furniture;
	self->shape.ent = self;
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
	ent->currentGridPosition = gridPosition;
//	bubbleEatBox.boxShape = gfc_shape_rect(attackTargetGridPosition.x * 32, attackTargetGridPosition.y * 32, 32, 32);
	//bubbleEatBox.boxShape.tag = Trigger;
	//bubbleEatBox.boxShape.identifier = Furniture;
	//bubbleEatBox.parent = ent;

	slog("1");
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/BubblesWithHandles.png",
		32,
		32,
		4,
		0);

	ent->startFrame = 3;
	ent->endFrame = 7;
	ent->typeNum = 1;
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

//	gf2d_space_add_static_shape(level_get_active_level()->space, bubbleEatBox.boxShape);
	gf2d_space_add_static_shape(level_get_active_level()->space, ent->shape);

	ent->data = (void*)&bubbleEatBox;
	ent->uniqueEntityTypeID = 2;		
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
	
	if (self->markedForDestruction == 0) {
		if (get_student_list()->count > 0) {
			int indexer = 0;
			for (indexer = get_student_list()->count - 1; indexer >= 0; indexer--) {
				Entity* normalStudent;
				normalStudent = gfc_list_get_nth(get_student_list(), indexer);
				if (!normalStudent)continue;
				if (normalStudent->markedForDestruction == 0 && normalStudent->uniqueEntityTypeID == 3)
				{
					if (vector2d_distance_between_less_than(self->position, normalStudent->position, 34.0)) {

						normalStudent->markedForDestruction = 2;//also deal damage
						play_sound("audio/StudentEaten.mp3", 2);
					}
					//normal_student_remove_from_list(Students, normalStudent);
				}
			}
		}
	}

}

void bubbles_update(Entity* self)
{
	if (self->markedForDestruction == 1)
	{
		self->startFrame = 0;
		self->endFrame = 2;
	//	BubbleEatBox* bbe;
	//	bbe = (struct BubbleEatBox*)self->data;
	//	if (self->timer == 0)
	//	{

		//	if (bbe)
		//	{
			//	slog("here");
			//	bbe->boxShape.identifier = Blank;
			//	bbe->parent = self;
			//	bbe->boxShape.tag = Trigger;
			//	self->data = (void*)&bbe;
			//}
		//}

		self->timer += 0.1;
		if (self->index >= 1) 
		{//currentlyy locked down

			if (GetPiperData()->mixingUpgrade == 1 && self->timer >= 64.0)
			{
				float randNum;
				randNum = gfc_random();

				float ranNum2;
				ranNum2 = gfc_random();
				if (randNum > 0.95)
				{
					if (ranNum2 > 0.5f) {
						Spellbook_New(vector2d((self->currentGridPosition.x) * 32, (self->currentGridPosition.y + 1) * 32),
							vector2d(self->currentGridPosition.x, self->currentGridPosition.y + 1));
					}
					else
					{
						Spellbook_New(vector2d((self->currentGridPosition.x) * 32, (self->currentGridPosition.y - 1) * 32),
							vector2d(self->currentGridPosition.x, self->currentGridPosition.y - 1));
					}
				}
				else if (randNum > .7) {
					if (ranNum2 > 0.5f) {
						Health_Pot_New(vector2d((self->currentGridPosition.x) * 32, (self->currentGridPosition.y + 1) * 32),
							vector2d(self->currentGridPosition.x, self->currentGridPosition.y + 1));
					}
					else
					{
						Health_Pot_New(vector2d((self->currentGridPosition.x - 1) * 32, (self->currentGridPosition.y - 1) * 32),
							vector2d(self->currentGridPosition.x - 1, (self->currentGridPosition.y - 1)));
					}
				}
				else {
					if (ranNum2 > 0.5f) {
						Broom_New(vector2d((self->currentGridPosition.x) * 32, (self->currentGridPosition.y -1)* 32),
							vector2d(self->currentGridPosition.x, self->currentGridPosition.y - 1));
					}
					else
					{
						Broom_New(vector2d((self->currentGridPosition.x) * 32, (self->currentGridPosition.y + 1) * 32),
							vector2d(self->currentGridPosition.x, self->currentGridPosition.y + 1));
					}
				}

				self->timer = 0;

			}
			return;
		}
		if (self->timer >= 128.0) {

			//if (bbe)
			//{
			//	bbe->boxShape.identifier = Furniture;
			//	bbe->parent = self;
			//	bbe->boxShape.tag = Trigger;
			//	self->data = (void*)&bbe;
			//}
			self->body.team = 3;
			self->timer = 0;
			self->startFrame = 3;
			self->endFrame = 7;
			self->markedForDestruction = 0;
			play_sound("audio/MagicReactive.mp3", 3);
		}
	}
}