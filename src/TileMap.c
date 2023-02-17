#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_config.h"
#include "gfc_list.h"

#include "gf2d_graphics.h"
#include "gf2d_draw.h"

#include "camera.h"
#include "TileMap.h"

void level_build(Level* level);

static Level* activeLevel = NULL;

typedef struct
{
    int graph_max;
    int graphSizeX;
    int graphSizeY;
    TileInfo* graph;
    
}GraphManager;

static GraphManager graph_manager;

TileInfo get_graph_node(int x, int y) {
    
    return graph_manager.graph[(y * (int)graph_manager.graphSizeX) + x];
}

Vector2D graph_to_world_pos(int x, int y)
{
    TileInfo tileToGet;
    tileToGet = get_graph_node(x, y);
    return vector2d(((tileToGet.coordinates.x * 32) + 16), ((tileToGet.coordinates.y * 32) + 16));

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


Level* level_load(const char* filename)
{
    int tile;
    int i, c;
    int j, d;
    int tileFPL;
    const char* str;
    SJson* json, * lj, * list, * row, * item;
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
    graph_manager.graph = gfc_allocate_array(sizeof(TileInfo), c * d);//NEW
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
            tileInfo_new(i, j, tile, (int)level->mapSize.x);

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
                graph_manager.graph[(i * (int)level->mapSize.x) + j].neighbours[0] = &graph_manager.graph[((i - 1) * (int)level->mapSize.x) + j];
            }

            if (i < c - 1) {
                graph_manager.graph[(i * (int)level->mapSize.x) + j].neighbours[2] = &graph_manager.graph[((i + 1) * (int)level->mapSize.x) + j];
            }

            if (j > 0) {
                graph_manager.graph[(i * (int)level->mapSize.x) + j].neighbours[1] = &graph_manager.graph[(i * (int)level->mapSize.x) + (j - 1)];
            }

            if (j < d - 1) {
                graph_manager.graph[(i * (int)level->mapSize.x) + j].neighbours[3] = &graph_manager.graph[(i * (int)level->mapSize.x) + (j + 1)];
            }

        }
    }
    
    sj_free(json);
    level_build(level);
    return level;
}

void tileInfo_new(int xCoord, int yCoord, int tileNum, int mapSize) {
    graph_manager.graph[(xCoord * (int)mapSize) + yCoord]._inuse = 1;
    graph_manager.graph[(xCoord * (int)mapSize) + yCoord].tileFrame = tileNum;
    graph_manager.graph[(xCoord * (int)mapSize) + yCoord].coordinates = vector2d(yCoord, xCoord);
}





int level_shape_clip(Level* level, Shape shape)
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
}

void level_build_clip_space(Level* level)
{
    Shape* shape;
    level->clips = gfc_list_new();
    int i, j;
    if (!level)return;
    for (j = 0; j < level->mapSize.y; j++)//j is row
    {
        for (i = 0; i < level->mapSize.x; i++)// i is column
        {
            if (level->tileMap[(j * (int)level->mapSize.x) + i] != 1)continue;//skip zero
            shape = gfc_allocate_array(sizeof(Shape), 1);
            if (!shape)continue;
            *shape = gfc_shape_rect(i * level->tileSize.x, j * level->tileSize.y, level->tileSize.x, level->tileSize.y);
            gfc_list_append(level->clips, shape);
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




float frame = 0;
void level_draw(Level* level)
{
    if (!level)return;
    if (!level->tileLayer)return;
    gf2d_sprite_draw_image(level->tileLayer, camera_get_draw_offset());
    int i;

 
  
    gf2d_draw_circle(graph_to_world_pos(x, y), 5, gfc_color(255, 0, 0, 255));
    
    for (int i = 0; i < 4; i++) {
        if (get_graph_node(x, y).neighbours[i] != NULL) {
            TileInfo *currentNeighbour;
            currentNeighbour = get_graph_node(x, y).neighbours[i];
            gf2d_draw_circle(graph_to_world_pos(currentNeighbour->coordinates.x, currentNeighbour->coordinates.y), 5, gfc_color(255, 0, 0, 255));
        }
    }
    slog("%i", graph_manager.graphSizeX);
    
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
    gfc_list_foreach(level->clips, free);
    gfc_list_delete(level->clips);
    free(level);
}

/*eol@eof*/