#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "gfc_text.h"
#include "gf2d_sprite.h"
#include "gfc_shape.h"
#include "gfc_list.h"

typedef struct
{
	int tileFrame;//the sprite?
	int solid; //0 false 1 true
	//TileInfo* neighbours[4];//pointer to a tiles neighbouts

}TileInfo;


typedef struct
{
	TextLine mapName;
	Sprite* tileSet;
	Vector2D tileSize;
	Vector2D completeMapSize;
	int* tileMapArray;
	Sprite* tileLayer;
	//List *clips;      //list of static shapes to clip against in the game world

}TileMap;


/**
 * @brief get the set active level pointer if it is set
 * @return NULL if no active level, or a pointer to it
 */
TileMap* tileMap_get_active_TileMap();

/**
 * @brief check if a shape clips any of the level static shapes
 * @param level the level to check
 * @param shape the shape to check with
 * @return 0 if there is no overlap, 1 if there is
 */
int tileMap_shape_clip(TileMap* tileMap, Shape shape);

TileMap* tileMap_new();

TileMap* tileMap_load(const char* filename);

void tileMap_set_active_TileMap(TileMap* tileMap);

TileMap* tileMap_load(const char* filename);

void tileMap_draw(TileMap* tileMap);

/**
*@brief clean up a previously allocated level
*/
void tileMap_free(TileMap* tileMap);

#endif // !__TILE_MAP_H__

