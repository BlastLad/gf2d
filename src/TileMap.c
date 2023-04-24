#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_config.h"
#include "gfc_list.h"

#include "gf2d_graphics.h"
#include "gf2d_draw.h"

#include "camera.h"
#include "Space.h";
#include "TileMap.h"

void level_build(Level* level);

static Level* activeLevel = NULL;

static GraphManager graph_manager;

GraphManager* get_graph() {
    return &graph_manager;
}

TileInfo get_graph_node(int x, int y) 
{
    return graph_manager.graph[(y * (int)graph_manager.graphSizeX) + x];
}

TileInfo* get_graph_node_pointer(int x, int y)
{
    TileInfo* tf;
    tf = &graph_manager.graph[(y * (int)graph_manager.graphSizeX) + x];
    return tf;
}

Vector2D graph_to_world_pos(int x, int y)
{
    TileInfo tileToGet;
    tileToGet = get_graph_node(x, y);
    return vector2d(((tileToGet.coordinates.x * 32) + 16), ((tileToGet.coordinates.y * 32) + 16));

}

TileInfo Get_Random_GridNode() 
{
    TileInfo tileToGet;
    int x, y;


    x = (rand() % ((graph_manager.graphSizeX - 1) - 1 + 0)) + 1;
    y = (rand() % ((graph_manager.graphSizeY - 1) - 1 + 0)) + 1;

    return graph_manager.graph[(y * (int)graph_manager.graphSizeX) + x];
}




Level* level_get_active_level()
{
    return activeLevel;
}

void level_set_active_level(Level* level)
{
    activeLevel = level;
}

void graph_close()
{
    if (graph_manager.graph)
    {
        free(graph_manager.graph);
    }
    slog("graph system closed");
    //entity_free_all();

}

void graph_manager_init(Uint32 max) 
{
    if (max <= 0)
    {
        slog("cannot inialize graph system: zero entities specified!");
        return;
    }
    graph_manager.graph = gfc_allocate_array(sizeof(TileInfo), max);
    if (!graph_manager.graph) {
        slog("failed to initialize graph system!");
        return;
    }
    graph_manager.graph_max = max;
    atexit(graph_close);//like an on disable but for the whole prog
    slog("graph system initialized");

}

SJson* map_getter(List* furniture)
{
    SJson* innerJson = sj_object_new();
    SJson* arrayw = sj_array_new();
    SJson* mapLayout = sj_array_new();
    SJson* furnitureLayout = sj_array_new();

    if (!innerJson) {
        slog("its fucked");
        return NULL;
    }

    int levelval;
    levelval = 1;
    
    sj_object_insert(innerJson, "name", sj_new_str("testerLevel"));
    sj_object_insert(innerJson, "tileSet", sj_new_str("images/CarpetTileSet.png"));
    sj_object_insert(innerJson, "tileFPL", sj_new_int(3));

    
    sj_array_append(arrayw, sj_new_int(32));
    sj_array_append(arrayw, sj_new_int(32));
    sj_object_insert(innerJson, "tileSize", arrayw);
    int c, d;
    c = graph_manager.graphSizeY;
    d = graph_manager.graphSizeX;
    for (int i = 0; i < c; i++) 
    {
        SJson* currentRow = sj_array_new();
        for (int j = 0; j < d; j++) 
        {
            int frameNum;
            frameNum = graph_manager.graph[(i * (int)graph_manager.graphSizeX) + j].tileFrame;
            sj_array_append(currentRow, sj_new_int(frameNum));
        }
        sj_array_append(mapLayout, currentRow);
    }

    for (int ii = 0; ii < gfc_list_get_count(furniture); ii++)
    {
        SJson* currentItem = sj_array_new();
        Entity* ent;
        ent = gfc_list_get_nth(furniture, ii);
        int newX;
        int newY, typeNum;
        newX = ent->currentGridPosition.x;
        newY = ent->currentGridPosition.y;
        typeNum = ent->typeNum;
        slog("Cheking ent things x %i, y %i", newX, newY);
        sj_array_append(currentItem, sj_new_int(typeNum));
        sj_array_append(currentItem, sj_new_int(newX));
        sj_array_append(currentItem, sj_new_int(newY));

        if (typeNum == 3) 
        {
            newX = ent->targetGridPosition.x;
            newY = ent->targetGridPosition.y;
            sj_array_append(currentItem, sj_new_int(newX));//targets
            sj_array_append(currentItem, sj_new_int(newY));
        }

        sj_array_append(furnitureLayout, currentItem);
    }




    sj_object_insert(innerJson, "tileMapArray", mapLayout);
    sj_object_insert(innerJson, "furnitureLayout", furnitureLayout);


    return innerJson;

}

void level_save(const char* filename, List* furniture)
{
    SJson* json;
    SJson* levelJson;  

    json = sj_object_new();
    levelJson = sj_new();

    if (!json || !levelJson) {
        slog("WARNING ITS BAD");
        return;
    }

    levelJson = map_getter(furniture);
    sj_object_insert(json, "level", levelJson);
    sj_save(json, "config/testSave.tilemap");


    sj_free(json);
  //  level = level_get_active_level();
   // if (!filename)return NULL;
   // json = sj_load(filename);
    //if (!json)return NULL;
}

Level* level_load(const char* filename)
{
    int tile;
    int i, c, enemyC;
    int j, d, enemyD;
    int tileFPL;
    const char* str;
    SJson* json, * lj, * list, * row, * item, * enemyList, * enemyRow;
    Level* level;
    if (!filename)return NULL;
    json = sj_load(filename);
    if (!json)return NULL;
    level = level_new();
    if (!level)
    {
        slog("failed to allocate new level");
        sj_free(json);
        return NULL;
    }
    lj = sj_object_get_value(json, "level");
    if (!lj)
    {
        slog("file %s missing level object", filename);
        sj_free(json);
        level_free(level);
        return NULL;
    }
    str = sj_object_get_value_as_string(lj, "name");
    if (str)gfc_line_cpy(level->name, str);
    sj_value_as_vector2d(sj_object_get_value(lj, "tileSize"), &level->tileSize);
    sj_object_get_value_as_int(lj, "tileFPL", &tileFPL);
    str = sj_object_get_value_as_string(lj, "tileSet");
    if (str)
    {
        level->tileSet = gf2d_sprite_load_all(str, (Sint32)level->tileSize.x, (Sint32)level->tileSize.y, tileFPL, 1);
    }
    list = sj_object_get_value(lj, "tileMapArray");
    c = sj_array_get_count(list);    
    row = sj_array_get_nth(list, 0);
    d = sj_array_get_count(row);
    if ((c * d) == 0)
    {
        slog("corrupt row or column count for %s level", filename);
        level_free(level);
        sj_free(json);
        return NULL;
    }
    level->mapSize.x = d;
    level->mapSize.y = c;
    slog("%i", d);
    slog("%i", c);
    level->tileMap = gfc_allocate_array(sizeof(int), c * d);
    graph_manager_init(c * d);
    graph_manager.graphSizeX = d;
    graph_manager.graphSizeY = c;
    if (!level->tileMap)
    {
        slog("failed to allocate tileMap for level %s", filename);
        level_free(level);
        sj_free(json);
        return NULL;
    }

    if (!graph_manager.graph) {
        slog("failed to allocate graph for level %s", filename);
        level_free(level);
        sj_free(json);
        graph_close();
        return NULL;
    }


    for (i = 0; i < c; i++)// i is row
    {
        row = sj_array_get_nth(list, i);
        if (!row)continue;
        d = sj_array_get_count(row);
        for (j = 0; j < d; j++)// j is column
        {
            item = sj_array_get_nth(row, j);
            if (!item)continue;
            tile = 0;//default
            sj_get_integer_value(item, &tile);
            level->tileMap[(i * (int)level->mapSize.x) + j] = tile;
            TileInfo* tileTest;
            tileTest = tileInfo_new(i, j, tile, (int)level->mapSize.x);
            if (!tileTest) {
                slog("failed to make a new tile at x %i y %i", j, i);
            }

        }
    }

    for (i = 0; i < c; i++)// i is row
    {
        row = sj_array_get_nth(list, i);
        if (!row)continue;
        d = sj_array_get_count(row);
        for (j = 0; j < d; j++)// j is column 
        {
            
            if (i > 0) {
                graph_manager.graph[(i * (int)level->mapSize.x) + j].neighbours[2] = &graph_manager.graph[((i - 1) * (int)level->mapSize.x) + j];
            }

            if (i < c - 1) {
                graph_manager.graph[(i * (int)level->mapSize.x) + j].neighbours[0] = &graph_manager.graph[((i + 1) * (int)level->mapSize.x) + j];
            }

            if (j > 0) {
                graph_manager.graph[(i * (int)level->mapSize.x) + j].neighbours[1] = &graph_manager.graph[(i * (int)level->mapSize.x) + (j - 1)];
            }

            if (j < d - 1) {
                graph_manager.graph[(i * (int)level->mapSize.x) + j].neighbours[3] = &graph_manager.graph[(i * (int)level->mapSize.x) + (j + 1)];
            }

        }
    }
    
    level_build(level);    
    sj_free(json);

    return level;
}

Entity* SpawnEnemy(int enemyVal, int gridX, int gridY, int tarX, int tarY)
{
    switch (enemyVal)
    {
    case 1:
        return Bubbles_New(graph_to_world_pos(gridX, gridY), vector2d(gridX, gridY), vector2d(gridX, gridY));
        break;
    case 2:
        return Rugby_New(graph_to_world_pos(gridX, gridY), vector2d(gridX, gridY));
        break;
    case 3:
        return karter_new(graph_to_world_pos(gridX, gridY), gridX, gridY, tarX, tarY);
        break;
    default:
        break;
    }
}

void ChangeTile(int prev_i_row, int prev_j_column, int newTileIndex) 
{
    int i;
    int j;

    i = prev_i_row;
    j = prev_j_column;
    int hello;
    hello = activeLevel->tileMap[(i * (int)activeLevel->mapSize.x) + j];
    slog("tile was %i hello", hello);

   activeLevel->tileMap[(i * (int)activeLevel->mapSize.x) + j] = newTileIndex;
    
   hello = activeLevel->tileMap[(i * (int)activeLevel->mapSize.x) + j];
   graph_manager.graph[(i * (int)activeLevel->mapSize.x) + j].tileFrame = newTileIndex;
   slog("tile is now %i hello", hello);


  level_art_free(activeLevel);
  
}

void level_art_free(Level* level)
{

    int i, j;

    if (level->tileLayer)gf2d_sprite_free(level->tileLayer);
    level->tileLayer = gf2d_sprite_new();
    if (!level->tileLayer)
    {
        slog("failed to create sprite for tileLayer");
        return;
    }

    if (level->tileLayer->surface)SDL_FreeSurface(level->tileLayer->surface);
    //create a surface the size we need it
    level->tileLayer->surface = gf2d_graphics_create_surface(level->tileSize.x * level->mapSize.x, level->tileSize.y * level->mapSize.y);
    if (!level->tileLayer->surface)
    {
        slog("failed to create tileLayer surface");
        return;
    }
    //make sure the surface is compatible with our graphics settings
    level->tileLayer->surface = gf2d_graphics_screen_convert(&level->tileLayer->surface);
    if (!level->tileLayer->surface)
    {
        slog("failed to create surface for tileLayer");
        return;
    }

    for (j = 0; j < graph_manager.graphSizeY; j++)//j is row
    {
        for (i = 0; i < graph_manager.graphSizeX; i++)// i is column
        {
            if (level->tileMap[(j * (int)level->mapSize.x) + i] <= 0)continue;//skip zero
            gf2d_sprite_draw_to_surface(
                level->tileSet,
                vector2d(i * level->tileSize.x, j * level->tileSize.y),
                NULL,
                NULL,
                get_graph_node(i, j).tileFrame - 1,
                level->tileLayer->surface);
        }
    }
    //convert it to a texture
    level->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), level->tileLayer->surface);
    SDL_SetTextureBlendMode(level->tileLayer->texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(level->tileLayer->texture,
        NULL,
        level->tileLayer->surface->pixels,
        level->tileLayer->surface->pitch);
    level->tileLayer->frame_w = level->tileLayer->surface->w;
    level->tileLayer->frame_h = level->tileLayer->surface->h;
    level->tileLayer->frames_per_line = 1;
}

TileInfo *tileInfo_new(int xCoord, int yCoord, int tileNum, int mapSize) {
    if (graph_manager.graph[(xCoord * (int)mapSize) + yCoord]._inuse == 0) {
        graph_manager.graph[(xCoord * (int)mapSize) + yCoord]._inuse = 1;
        graph_manager.graph[(xCoord * (int)mapSize) + yCoord].tileFrame = tileNum;
        graph_manager.graph[(xCoord * (int)mapSize) + yCoord].coordinates.x = yCoord;
        graph_manager.graph[(xCoord * (int)mapSize) + yCoord].coordinates.y = xCoord;
        return &graph_manager.graph[(xCoord * (int)mapSize) + yCoord];
    }
    
    return NULL;
}

/*int level_shape_clip(Level* level, Shape shape)
{
    int i, c;
    Shape* clip;
    if (!level)return 0;
    c = gfc_list_get_count(level->clips);
    for (i = 0; i < c; i++)
    {
        clip = gfc_list_get_nth(level->clips, i);
        if (!clip)continue;
        if (gfc_shape_overlap(*clip, shape))return 1;
    }
    return 0;
}*/

int level_shape_clip(Level* level, Shape shape)
{
    List* collisions;
    if (!level)return 0;

    collisions = gf2d_space_static_shape_check(level->space, shape, NULL);
    if (collisions)
    {
        gfc_list_delete(collisions);
        return 1;
    }
    return 0;
}


void level_build_clip_space(Level* level)
{
    Shape shape;
    //level->clips = gfc_list_new();
    int i, j;
    if ((!level) || (!level->tileLayer))return;

    level->space = gf2d_space_new_full(
        3,
        gfc_rect(0, 0, level->tileLayer->frame_w, level->tileLayer->frame_h),
        0.01,
        vector2d(0, 0),
        1,
        1,//slop
        1,//use hash or not
        vector2d(128, 128));//could be problems with this

    if (!level->space)return;

    for (j = 0; j < level->mapSize.y; j++)//j is row
    {
        for (i = 0; i < level->mapSize.x; i++)// i is column
        {
            if (level->tileMap[(j * (int)level->mapSize.x) + i] == 1) {
                //shape = gfc_allocate_array(sizeof(Shape), 1);
                //if (!shape)continue;
                shape = gfc_shape_rect(i * level->tileSize.x, j * level->tileSize.y, level->tileSize.x, level->tileSize.y);
                gf2d_space_add_static_shape(level->space, shape);
            }
            //gfc_list_append(level->clips, shape);
        }
    }
}

static int x;
static int y;

void level_build(Level* level)
{
    x = 0;
    y = 0;

    int i, j;
    if (!level)return;
    if (level->tileLayer)gf2d_sprite_free(level->tileLayer);
    level->tileLayer = gf2d_sprite_new();
    if (!level->tileLayer)
    {
        slog("failed to create sprite for tileLayer");
        return;
    }
    // if there is a default surface, free it
    if (level->tileLayer->surface)SDL_FreeSurface(level->tileLayer->surface);
    //create a surface the size we need it
    level->tileLayer->surface = gf2d_graphics_create_surface(level->tileSize.x * level->mapSize.x, level->tileSize.y * level->mapSize.y);
    if (!level->tileLayer->surface)
    {
        slog("failed to create tileLayer surface");
        return;
    }
    //make sure the surface is compatible with our graphics settings
    level->tileLayer->surface = gf2d_graphics_screen_convert(&level->tileLayer->surface);
    if (!level->tileLayer->surface)
    {
        slog("failed to create surface for tileLayer");
        return;
    }
    //draw the tile sprite to the surface
    for (j = 0; j < graph_manager.graphSizeY; j++)//j is row
    {
        for (i = 0; i < graph_manager.graphSizeX; i++)// i is column
        {
            if (level->tileMap[(j * (int)level->mapSize.x) + i] <= 0)continue;//skip zero
            gf2d_sprite_draw_to_surface(
                level->tileSet,
                vector2d(i * level->tileSize.x, j * level->tileSize.y),
                NULL,
                NULL,
                get_graph_node(i, j).tileFrame - 1,
                level->tileLayer->surface);
        }
    }
    //convert it to a texture
    level->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), level->tileLayer->surface);
    SDL_SetTextureBlendMode(level->tileLayer->texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(level->tileLayer->texture,
        NULL,
        level->tileLayer->surface->pixels,
        level->tileLayer->surface->pitch);
    level->tileLayer->frame_w = level->tileLayer->surface->w;
    level->tileLayer->frame_h = level->tileLayer->surface->h;
    level->tileLayer->frames_per_line = 1;
    camera_set_world_size(vector2d(level->tileLayer->frame_w, level->tileLayer->frame_h));
    level_build_clip_space(level);
}



int GetWeightForTileIndex(int tileFrame) {

    switch (tileFrame)
    {
    case 1:
        return 99;
    case 17:
        return 17;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        return 1;
    default:
       return 1;
    }
    return 99;
}

void xy(void* data) 
{
    TileInfo* v;
    if (data) {
        v = data;
        slog("Hello %i", v->coordinates.x);
    }
}


typedef struct
{
    Vector2D coordinates;
    TileInfo* tileInfo;
    float distanceValue;
}dist;

typedef struct
{
    Vector2D currentCoordinates;
    TileInfo* tileInfo;
    TileInfo* prevNode;
}prev;


void PathFinding(int srcX, int srcY, int destX, int destY) 
{
    TileInfo srcTile; 
    srcTile = get_graph_node(srcX, srcY);
    TileInfo dstTile; 
    dstTile = get_graph_node(destX, destY);
  //  TileInfo* unvisited;

  //  unvisited = gfc_allocate_array(sizeof(TileInfo), graph_manager.graph_max);


    //gfc_list_foreach_context(graph_manager.graph, xy, )
    //gfc_list_foreach(graph_manager.graph, xy);

    List* unvistedList;
   // List* distList;
  //  List* prevList;

    dist* distArray;
    distArray = gfc_allocate_array(sizeof(dist), graph_manager.graph_max);
    prev* prevArray;
    prevArray = gfc_allocate_array(sizeof(prev), graph_manager.graph_max);


    unvistedList = gfc_list_new();
   // distList = gfc_list_new();
   // prevList = gfc_list_new();

    int i;
    int j;
    TileInfo* tobeAppend;
    dist distanceItem;
    prev prevItem;
    TileInfo newTile;


    //Initalizes the sourceTiles items in the distacne and prev arrays
    vector2d_copy(distanceItem.coordinates, srcTile.coordinates);
    vector2d_copy(prevItem.currentCoordinates, srcTile.coordinates);
    distanceItem.distanceValue = 0;
    distanceItem.tileInfo = &srcTile;
    prevItem.tileInfo = &srcTile;
    prevItem.prevNode = NULL;

    distArray[(srcY * (int)graph_manager.graphSizeX) + srcX] = distanceItem;
    prevArray[(srcY * (int)graph_manager.graphSizeX) + srcX] = prevItem;
    
    slog("graph man max %i", graph_manager.graph_max);
    int numberAdded = 1;
    //gfc_list_append(distList, &distanceItem);
   // gfc_list_append(prevList, &prevItem);
    int hjx;
    int hjy;
    for (j = 0; j < graph_manager.graphSizeY; j++)//j is row
    {
        for (i = 0; i < graph_manager.graphSizeX; i++)// i is column
        {   
            tobeAppend = get_graph_node_pointer(i, j);
            hjx = tobeAppend->coordinates.x;
            hjy = tobeAppend->coordinates.y;
            
            slog("OK Number 1 %i %i", hjx, hjy);

            if (tobeAppend->coordinates.x == srcX && tobeAppend->coordinates.y == srcY) 
            {
                
            }
            else 
            {
                //initalizes the dist and pre arrays with 99 and NULL as defaults
                vector2d_copy(distanceItem.coordinates, tobeAppend->coordinates);
                vector2d_copy(prevItem.currentCoordinates, tobeAppend->coordinates);

                hjx = distanceItem.coordinates.x;
                hjy = distanceItem.coordinates.y;
               // slog("OK Number 1 post %i %i", hjx, hjy);
                distanceItem.distanceValue = 999;
                prevItem.prevNode = NULL;
                distanceItem.tileInfo = tobeAppend;
                prevItem.tileInfo = tobeAppend;
                // gfc_list_append(distList, &distanceItem);
                // gfc_list_append(prevList, &prevItem);
                distArray[(j * (int)graph_manager.graphSizeX) + i] = distanceItem;
                prevArray[(j * (int)graph_manager.graphSizeX) + i] = prevItem;
                numberAdded += 1;
            }

            gfc_list_append(unvistedList, tobeAppend);
          //  unvisited[(j * (int)graph_manager.graphSizeX) + i] = get_graph_node(i, j);
        }
    }



    slog("OK Number unvisted list %i", unvistedList->count);
    //OK UP TO HERE
    TileInfo* u;
    TileInfo* possibleU;
    int possibleUX;
    int possibleUY;
    int uY;
    int uX;
    numberAdded = 0;
    int val;
    val = 0;

  /*  if (Students->count > 0) {
        for (indexer = Students->count - 1; indexer >= 0; indexer--) {
            normalStudent = gfc_list_get_nth(Students, indexer);
            if (!normalStudent)continue;
            if (normalStudent->markedForDestruction > 0) {
                removeStudent(Students, normalStudent);
            }
        }
    } */

    while (unvistedList->count > 0)
    {
        u = NULL;
        val++;
        int k;
        for (k = unvistedList->count - 1; k >= 0; k--) 
        {
            //k is possible u
            possibleU = gfc_list_get_nth(unvistedList, k);


            possibleUX = possibleU->coordinates.x;
            possibleUY = possibleU->coordinates.y;
            slog("OK Number 2: k val: %i | %i coords %i %i", k, val, possibleUX, possibleUY);


            if (u == NULL)
            {

                u = gfc_list_get_nth(unvistedList, k);
                uY = u->coordinates.y;
                uX = u->coordinates.x;


            }
            else if (distArray[(possibleUY * (int)graph_manager.graphSizeX) +
                possibleUX].distanceValue <
                distArray[(uY * (int)graph_manager.graphSizeX) + uX].distanceValue) 
            {
                slog("OK Number 2.5");

                u = gfc_list_get_nth(unvistedList, k);
                uY = u->coordinates.y;
                uX = u->coordinates.x;
            }
        }

        if (uX == dstTile.coordinates.x && uY == dstTile.coordinates.y) 
        {
            slog("once more");
            slog("BREAKING OUT %i %i", uX, uY);
            break;
        }

        gfc_list_delete_data(unvistedList, u);

        int nodeNeighbourIndex;
        for (nodeNeighbourIndex = 0; nodeNeighbourIndex < 4; nodeNeighbourIndex++) 
        {
            float alt;
            float nAlt;
            TileInfo* currentNeighbour;
            int currentNeighbourX;
            int currentNeighbourY;

      


           /* for (i = 0; i < 4; i++) {
                if (get_graph_node(x, y).neighbours[i] != NULL) {
                    TileInfo* currentNeighbour;
                    currentNeighbour = get_graph_node(x, y).neighbours[i];
                    gf2d_draw_circle(graph_to_world_pos(currentNeighbour->coordinates.x, currentNeighbour->coordinates.y), 5, gfc_color(255, 0, 0, 255));
                }
            }*/
            slog("Number X Coords %i %i", uX, uY);
            if (u->neighbours[nodeNeighbourIndex] != NULL)
            {
                currentNeighbour = u->neighbours[nodeNeighbourIndex];
                currentNeighbourX = currentNeighbour->coordinates.x;
                currentNeighbourY = currentNeighbour->coordinates.y;
                nAlt = GetWeightForTileIndex(currentNeighbour->tileFrame);
                slog("Number 3 %i tile num %i", currentNeighbour->tileFrame, val);
                //distArray[(uY * (int)graph_manager.graphSizeX) + uX].distanceValue

                alt = distArray[(uY * (int)graph_manager.graphSizeX) + uX].distanceValue + nAlt;
                slog("OK Number 3 %f", alt);

                if (alt < distArray[(currentNeighbourY * (int)graph_manager.graphSizeX + currentNeighbourX)].distanceValue)
                {

                    distanceItem.distanceValue = alt;
                    distanceItem.tileInfo = currentNeighbour;
                    vector2d_copy(distanceItem.coordinates, currentNeighbour->coordinates);
                    //coords

                    distArray[(currentNeighbourY * (int)graph_manager.graphSizeX + currentNeighbourX)] = distanceItem;
                    prevItem.prevNode = u;
                    vector2d_copy(prevItem.currentCoordinates, currentNeighbour->coordinates);
                    prevItem.tileInfo = currentNeighbour;

                    prevArray[(currentNeighbourY * (int)graph_manager.graphSizeX + currentNeighbourX)] = prevItem;
                    slog("OK Number 4");
                    //remap it
                }
              //  slog("OK Number 4");
            }
        }
    } 
   
    int debugI;
    int debugJ;
    float finalVal;
    Vector2D vec;
    for (debugJ = 0; debugJ < graph_manager.graphSizeY; debugJ++) 
    {       
        for (debugI = 0; debugI < graph_manager.graphSizeX; debugI++)
        {
            /*vec = distArray[(debugJ * (int)graph_manager.graphSizeX + debugI)].coordinates;
            finalVal = distArray[(debugJ * (int)graph_manager.graphSizeX + debugI)].distanceValue;
            slog("hello %f %f And dist val: %f", vec.x, vec.y, finalVal);
            vec = distArray[(debugJ * (int)graph_manager.graphSizeX + debugI)].tileInfo->coordinates;
            slog("double check %f %f And dist val: %f", vec.x, vec.y, finalVal);*/

            vec = prevArray[(debugJ * (int)graph_manager.graphSizeX + debugI)].currentCoordinates;
            slog("hello %f %f", vec.x, vec.y);
            if (prevArray[(debugJ * (int)graph_manager.graphSizeX + debugI)].prevNode != NULL) 
            {
                vec = prevArray[(debugJ * (int)graph_manager.graphSizeX + debugI)].prevNode->coordinates;
                slog("PREVIOUS NODE WAS %f %f", vec.x, vec.y);
            }
            vec = prevArray[(debugJ * (int)graph_manager.graphSizeX + debugI)].tileInfo->coordinates;
            slog("double check %f %f", vec.x, vec.y);

        }      

    }

    vec = distArray[(3 * (int)graph_manager.graphSizeX + 3)].coordinates;
    slog("hello final %f %f", vec.x, vec.y);



    List* currentPath;
    slog("OK Number 5");
    currentPath = gfc_list_new();
    slog("OK Number 6");
    TileInfo* curr = &dstTile;
    int currX;
    int currY;
    currX = curr->coordinates.x;
    currY = curr->coordinates.y;
    while (prevArray[(currY * (int)graph_manager.graphSizeX + currX)].prevNode != NULL)
    {
        slog("OK Number 7");

        gfc_list_append(currentPath, curr);
        slog("OK Number 8");
        curr = prevArray[(currY * (int)graph_manager.graphSizeX + currX)].prevNode;
        currX = curr->coordinates.x;
        currY = curr->coordinates.y;
        slog("OK Number 9");

        //slog("OK Number 5");

    }
  
}


void get_next_carpet_tile(float x, float y, Entity *ent) 
{    
    int j;
    Vector2D vectorToReturn;
    vectorToReturn = vector2d(x, y);


    for (j = 0; j < 4; j++) 
    {
       
        if (get_graph_node(ent->targetGridPosition.x, ent->targetGridPosition.y).neighbours[j] != NULL) 
        {
            TileInfo* currentNeighbour;
            currentNeighbour = get_graph_node(ent->targetGridPosition.x, ent->targetGridPosition.y).neighbours[j];
        
            if (currentNeighbour->tileFrame >= 2 && currentNeighbour->tileFrame <= 12) //is carpet
            {
                if (currentNeighbour->coordinates.x != x || currentNeighbour->coordinates.y != y) 
                {//if the currentNeighbour of the target is not the units current position
                    if (ent->index == 0) //straight mover
                    {
                        if (ent->currentGridPosition.x == ent->targetGridPosition.x) {//moving straight vertically
                            if (currentNeighbour->coordinates.x == ent->currentGridPosition.x) 
                            {
                                vector2d_copy(ent->targetGridPosition, currentNeighbour->coordinates);//copy 
                                return;
                            }
                        }
                        else if (ent->currentGridPosition.y == ent->targetGridPosition.y)//moving straight horizontally
                        {
                            if (currentNeighbour->coordinates.y == ent->currentGridPosition.y)
                            {
                                vector2d_copy(ent->targetGridPosition, currentNeighbour->coordinates);//copy 
                                return;
                            }
                        }
                    }
                    else if (ent->index == 1) 
                    {
                   

                        if (ent->counter > 16)
                        {
                            ent->index = 0;//for lost male student;
                            j = 0;
                        }


                            int currenttileFrame;
                            currenttileFrame = get_graph_node(ent->currentGridPosition.x, ent->currentGridPosition.y).tileFrame;//example 2 before t tile

                            if (currenttileFrame == currentNeighbour->tileFrame) //this would be the 2 after the t piece
                            {
                                vectorToReturn = currentNeighbour->coordinates;
                           
                            }
                            else {
                                vectorToReturn = currentNeighbour->coordinates;//copy
                                break;
                            }
                    }
                    
                                                  
                }
            }
        }
    }


    if (ent->index == 1) 
    {
        vector2d_copy(ent->targetGridPosition, vectorToReturn);//copy but dont return
    }
}


void get_next_previous_tile(float x, float y, Entity* ent)
{
    int j;
    Vector2D vectorToReturn;
    vectorToReturn = vector2d(x, y);
    for (j = 0; j < 4; j++)
    {

        if (get_graph_node(ent->currentGridPosition.x, ent->currentGridPosition.y).neighbours[j] != NULL)
        {
            TileInfo* currentNeighbour;
            currentNeighbour = get_graph_node(ent->currentGridPosition.x, ent->currentGridPosition.y).neighbours[j];

            if (currentNeighbour->tileFrame >= 2 && currentNeighbour->tileFrame <= 12) //is carpet
            {
                if (currentNeighbour->coordinates.x != x || currentNeighbour->coordinates.y != y)
                {//if the currentNeighbour of the self is not the units target position
                    if (ent->index == 0) //straight mover
                    {
                        if (ent->currentGridPosition.x == ent->targetGridPosition.x) {//moving straight vertically
                            if (currentNeighbour->coordinates.x == ent->currentGridPosition.x)
                            {
                                vector2d_copy(ent->targetGridPosition, currentNeighbour->coordinates);//copy 
                                return;
                            }
                        }
                        else if (ent->currentGridPosition.y == ent->targetGridPosition.y)//moving straight horizontally
                        {
                            if (currentNeighbour->coordinates.y == ent->currentGridPosition.y)
                            {
                                vector2d_copy(ent->targetGridPosition, currentNeighbour->coordinates);//copy 
                                return;
                            }
                        }
                    }
                    else if (ent->index == 1)
                    {


                        int currenttileFrame;
                        currenttileFrame = get_graph_node(ent->currentGridPosition.x, ent->currentGridPosition.y).tileFrame;//example 2 before t tile

                        if (currenttileFrame == currentNeighbour->tileFrame) //this would be the 2 after the t piece
                        {
                            vectorToReturn = currentNeighbour->coordinates;

                        }
                        else {
                            vectorToReturn = currentNeighbour->coordinates;//copy
                            break;
                        }
                    }


                }
            }
        }
    }


    if (ent->index == 1)
    {
        vector2d_copy(ent->targetGridPosition, vectorToReturn);//copy but dont return
    }
}

float frame = 0;
void level_draw(Level* level)
{
    if (!level)return;
    if (!level->tileLayer)return;
    gf2d_sprite_draw_image(level->tileLayer, camera_get_draw_offset());
    int i;

 
  
    gf2d_draw_circle(graph_to_world_pos(x, y), 5, gfc_color(255, 0, 0, 255));
    
    for (i = 0; i < 4; i++) {
        if (get_graph_node(x, y).neighbours[i] != NULL) {
            TileInfo *currentNeighbour;
            currentNeighbour = get_graph_node(x, y).neighbours[i];
            gf2d_draw_circle(graph_to_world_pos(currentNeighbour->coordinates.x, currentNeighbour->coordinates.y), 5, gfc_color(255, 0, 0, 255));
        }
    }
    frame += 0.1;
    if (frame >= 2) {

        frame = 0;
        if (x < graph_manager.graphSizeX - 1) {
            x++;
        }
        else {
            x = 0;
            y++;
        }

        if (y < graph_manager.graphSizeY) {

        }
        else {
            y = 0;
            x = 0;
        }

    }

}

//The start of updating the space/level/scene
void tileMap_Update(Level* level) {
    if (!level)return;
    gf2d_space_update(level->space);
}


Level* level_new()
{
    Level* level;
    level = gfc_allocate_array(sizeof(Level), 1);
   
    return level;
}

void level_free(Level* level)
{
    if (!level)return;
    if (level->tileSet)gf2d_sprite_free(level->tileSet);
    if (level->tileLayer)gf2d_sprite_free(level->tileLayer);
    if (level->tileMap)free(level->tileMap);
    if (level->space)gf2d_space_free(level->space);
    if (level->clips) 
    {
        gfc_list_foreach(level->clips, free);
        gfc_list_delete(level->clips);
    }
    free(level);
}



//adds the body to the space buckets
void level_add_entity(Level* level, Entity* entity) 
{
    if ((!level) || (!entity))return;
    gf2d_space_add_body(level->space, &entity->body, entity);
}

void level_remove_entity(Entity* ent)
{
    if (!ent)return;
    if (!level_get_active_level()->space)
    {//nothing to do
        return;
    }
    
    gf2d_space_remove_body(level_get_active_level()->space, &ent->body);
}

/*eol@eof*/