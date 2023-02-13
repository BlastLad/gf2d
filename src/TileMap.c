#ifndef __TILE_MAP_C__
#define __TILE_MAP_C__

#include "simple_json.h"
#include "simple_logger.h"
#include "gf2d_graphics.h"

#include "camera.h"
#include "gfc_config.h"
#include "TileMap.h"
#include "gfc_list.h"

void tileMap_build(TileMap* tileMap);

static TileMap* activeLevel = NULL;

TileMap* tileMap_get_active_TileMap()
{
	return activeLevel;
}

void tileMap_set_active_TileMap(TileMap* level)
{
	activeLevel = level;
}

TileMap* tileMap_new()
{
	TileMap* tileMap;
	tileMap = gfc_allocate_array(sizeof(TileMap), 1);
	//tileMap->clips = gfc_list_new();
	return tileMap;
}

TileMap* tileMap_load(const char* filename)
{
	int tile;
	int i, c;
	int j, d;

	int tileFPL;
	const char* str;
	SJson* json, * lj, * list, * row, * item;
	Vector2D tileSize = { 0 };
	TileMap* tileMap;
	if (!filename)return NULL;
	json = sj_load(filename);
	if (!json)return NULL;

	tileMap = tileMap_new();
	if (!tileMap) {
		slog("failed to allocate new level");
		sj_free(json);
		return NULL;
	}
	lj = sj_object_get_value(json, "level");//hard coded value

	if (!lj)
	{
		slog("file %s missing level objet", filename);
		sj_free(json);
		tileMap_free(tileMap);
		return NULL;
	}

	str = sj_object_get_value_as_string(lj, "name");
	if (str)gfc_line_cpy(tileMap->mapName, str);
	sj_value_as_vector2d(sj_object_get_value(lj, "tileSize"), &tileSize);	
	sj_object_get_value_as_int(lj, "tileFPL", &tileFPL);
	str = sj_object_get_value_as_string(lj, "tileSet");

	if (str)
	{
		tileMap->tileSet = gf2d_sprite_load_all(str, (Sint32)tileSize.x, (Sint32)tileSize.y, tileFPL, 1);
	}
	list = sj_object_get_value(lj, "tileMapArray");
	c = sj_array_get_count(list);
	row = sj_array_get_nth(list, 0);
	d = sj_array_get_count(row);

	if ((c * d) == 0)
	{
		slog("corrupt row or column count %s level", filename);
		tileMap_free(tileMap);
		sj_free(json);
		return NULL;
	}

	tileMap->completeMapSize.x = d;
	tileMap->completeMapSize.y = c;	

	tileMap->tileMapArray = gfc_allocate_array(sizeof(int), c * d);

	if (!tileMap->tileMapArray) {
		slog("failed to allocate tileMap for level %s", filename);
		tileMap_free(tileMap);
		sj_free(json);
		return NULL;
	}

	for (i = 0; i < c; i++)
	{
		row = sj_array_get_nth(list, i);
		if (!row)continue;
		d = sj_array_get_count(row);
		for (j = 0; j < d; j++) {
			item = sj_array_get_nth(row, j);
			if (!item) continue;
			tile = 0;
			sj_get_integer_value(item, &tile);
			tileMap->tileMapArray[(i * (int)tileMap->completeMapSize.x) + j] = tile;
		}
	}
	sj_free(json);
	tileMap_build(tileMap);
	return tileMap;
}



void tileMap_free(TileMap* tileMap) {
	if (!tileMap) return;
	if (tileMap->tileSet)gf2d_sprite_free(tileMap->tileSet);
	if (tileMap->tileLayer)gf2d_sprite_free(tileMap->tileLayer);
	if (tileMap->tileMapArray)free(tileMap->tileMapArray);
	//gfc_list_foreach(tileMap->clips, free);
	//gfc_list_delete(tileMap->clips);
	free(tileMap);

}

void level_build_clip_space(TileMap* level)
{
	Shape* shape;
	int i, j;
	if (!level)return;
	for (j = 0; j < level->completeMapSize.y; j++)//j is row
	{
		for (i = 0; i < level->completeMapSize.x; i++)// i is column
		{
			if (level->tileMapArray[(j * (int)level->completeMapSize.x) + i] <= 0)continue;//skip zero
			shape = gfc_allocate_array(sizeof(Shape), 1);
			if (!shape)continue;
			*shape = gfc_shape_rect(i * level->tileSize.x, j * level->tileSize.y, level->tileSize.x, level->tileSize.y);
			//gfc_list_append(level->clips, shape);
		}
	}
}

void tileMap_build(TileMap* tileMap)
{
	int i, j;
	if (!tileMap)return;
	if (tileMap->tileLayer)gf2d_sprite_free(tileMap->tileLayer);
	tileMap->tileLayer = gf2d_sprite_new();
	if (!tileMap->tileLayer)
	{
		slog("failed to create sprite for tileLayer");
		return;
	}
	// if there is a default surface, free it
	if (tileMap->tileLayer->surface)SDL_FreeSurface(tileMap->tileLayer->surface);
	//create a surface the size we need it
	tileMap->tileLayer->surface = gf2d_graphics_create_surface(tileMap ->tileSize.x * tileMap->completeMapSize.x, tileMap->tileSize.y * tileMap->completeMapSize.y);
	if (!tileMap->tileLayer->surface)
	{
		slog("failed to create tileLayer surface");
		return;
	}
	//make sure the surface is compatible with our graphics settings
	tileMap->tileLayer->surface = gf2d_graphics_screen_convert(&tileMap->tileLayer->surface);
	if (!tileMap->tileLayer->surface)
	{
		slog("failed to create surface for tileLayer");
		return;
	}
	//draw the tile sprite to the surface
	for (j = 0; j < tileMap->completeMapSize.y; j++)//j is row
	{
		for (i = 0; i < tileMap->completeMapSize.x; i++)// i is column
		{
			if (tileMap->tileMapArray[(j * (int)tileMap->completeMapSize.x) + i] <= 0) 
			{
				//slog("Was zero continue %i", (j * (int)tileMap->completeMapSize.x) + i);
				continue;//skip zero
			}
			//slog("Was OK %i", (j * (int)tileMap->completeMapSize.x) + i);
			gf2d_sprite_draw_to_surface(
				tileMap->tileSet,
				vector2d(i * tileMap->tileSize.x, j * tileMap->tileSize.y),
				NULL,
				NULL,
				tileMap->tileMapArray[(j * (int)tileMap->completeMapSize.x) + i] - 1,
				tileMap->tileLayer->surface);
		}
	}
	//convert it to a texture
	tileMap->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), tileMap->tileLayer->surface);
	SDL_SetTextureBlendMode(tileMap->tileLayer->texture, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(tileMap->tileLayer->texture,
		NULL,
		tileMap->tileLayer->surface->pixels,
		tileMap->tileLayer->surface->pitch);
	tileMap->tileLayer->frame_w = tileMap->tileLayer->surface->w;
	tileMap->tileLayer->frame_h = tileMap->tileLayer->surface->h;
	tileMap->tileLayer->frames_per_line = 1;
	camera_set_world_size(vector2d(tileMap->tileLayer->frame_w, tileMap->tileLayer->frame_h));
	//level_build_clip_space(tileMap);
}

void tileMap_draw(TileMap* tileMap) 
{
	if (!tileMap) 
	{
	//	slog("No TileMap");
		return;
	}
	if (!tileMap->tileLayer) 
	{
		//slog("No TileLayer");
		return;
	}
	gf2d_sprite_draw_image(tileMap->tileLayer, camera_get_draw_offset());
}


#endif // !__TILE_MAP_C__