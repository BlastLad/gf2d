#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "gfc_text.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "Space.h"
#include "Entity.h"


typedef struct TileInfo
{
    int solid;  // if true the tile cannot be walked through
    Bool _inuse;
    int tileFrame;   
    Vector2D coordinates;
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
    Space* space;
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

Entity* SpawnEnemy(int enemyVal, int gridX, int gridY, int tarX, int tarY);

void level_save(const char* filename, List* furniture);

TileInfo get_graph_node(int x, int y);

List* PathFinding(int srcX, int srcY, int destX, int destY);

void get_next_carpet_tile(float x, float y, Entity* ent);

void get_next_previous_tile(float x, float y, Entity* ent);

Vector2D graph_to_world_pos(int x, int y);


TileInfo Get_Random_GridNode();
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

void level_art_free(Level* level);


void ChangeTile(int prev_i_row, int prev_j_column, int newTileIndex);

/**
 * @brief draw a level
 * @param level the level to draw
 */
void level_draw(Level* level);

void tileMap_Update(Level* level);

void level_add_entity(Level* level, Entity* entity);

void level_remove_entity(Entity* ent);

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

