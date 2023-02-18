#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "gfc_text.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "Entity.h"


typedef struct TileInfo
{
    int solid;  // if true the tile cannot be walked through
    Bool _inuse;
    int tileFrame;   
    int coordinateX;
    int coordinateY;
    struct TileInfo *neighbours[4];//order is 0 is left 1 is up 2 is right and 3 is down
}TileInfo;

typedef struct
{
    TextLine    name;
    Sprite* tileSet;
    Vector2D    tileSize;    //the width and height of the tile map
    Vector2D    mapSize;    //the width and height of the tile map
    int* tileMap;    //WTF???
    Sprite* tileLayer;  //TBD
    List *clips;      //list of static shapes to clip against in the game world
}Level;

typedef struct
{
    int graph_max;
    int graphSizeX;
    int graphSizeY;
    TileInfo* graph;

}GraphManager;

GraphManager* get_graph();

TileInfo get_graph_node(int x, int y);

void get_next_carpet_tile(int x, int y, Entity* ent);

Vector2D graph_to_world_pos(int x, int y);

/**
 * @brief get the set active level pointer if it is set
 * @return NULL if no active level, or a pointer to it
 */
Level* level_get_active_level();

/**
 * @brief check if a shape clips any of the level static shapes
 * @param level the level to check
 * @param shape the shape to check with
 * @return 0 if there is no overlap, 1 if there is
 */
int level_shape_clip(Level* level, Shape shape);

/**
 * @brief set the active level (or turn it off)
 * @param level the level to set it too (or NULL to turn off the level)
 */
void level_set_active_level(Level* level);

/**
 * @brief load a level from a json file
 * @param filename path to the json file containing level data
 * @return NULL on not found or other error, or the loaded level
 */
Level* level_load(const char* filename);

/**
 * @brief draw a level
 * @param level the level to draw
 */
void level_draw(Level* level);


TileInfo *tileInfo_new(int xCoord, int yCoord, int tileNum, int mapSize);


/**
 * @brief allocate a blank level struct
 * @return NULL on error or an intialized level
 */
Level* level_new();

/**
 * @brief clean up a previously allocated level
 */
void level_free(Level* level);

#endif

