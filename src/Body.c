#include <stdlib.h>
#include "simple_logger.h"

#include "gfc_config.h"
#include "simple_json.h"
#include "ClipLayers.h"

#include "gf2d_draw.h"
#include "Body.h"



typedef struct
{
    Uint32 body_max;
    Body* body_list;
    SJson* body_def;
}BodyManager;

static BodyManager body_manager = { 0 };

Uint8 gf2d_body_shape_collide(Body* a, Shape* s, Vector2D* poc, Vector2D* normal);

void gf2d_body_clear(Body* body)
{
    if (!body)return;
    body->inuse = false;
    memset(body, 0, sizeof(Body));
}

void gf2d_body_push(Body* body, Vector2D direction, float force)
{
    if (!body)return;
    if (body->mass != 0)
    {
        force = force / body->mass;
    }
    vector2d_set_magnitude(&direction, force);
    vector2d_add(body->velocity, body->velocity, direction);
}

void gf2d_body_draw(Body* body, Vector2D offset)
{
    Color color;
    Shape shape;
    Vector2D center;
    if (!body)return;
    color = gfc_color8(0, 255, 255, 255);
    // draw center point
    vector2d_add(center, body->position, offset);
    gf2d_draw_circle(center, 2, color);

    color = gfc_color8(255, 0, 255, 255);
    gfc_shape_copy(&shape, *body->shape);
    gfc_shape_move(&shape, body->position);
    gf2d_draw_shape(shape, color, offset);
}

void gf2d_body_set(
    Body* body,
    const char* name,
    Uint8       worldclip,
    Uint32      cliplayer,
    Uint32      touchlayer,
    Uint32      team,
    Vector2D    position,
    Vector2D    velocity,
    float       mass,
    float       gravity,
    float       elasticity,
    Shape* shape,
    void* data,
    int         (*touch)(struct DynamicBody* self, List* collision))
{
    if (!body)return;
    body->cliplayer = cliplayer;
    body->touchlayer = touchlayer;
    body->team = team;
    body->worldclip = worldclip;
    vector2d_copy(body->position, position);
    vector2d_copy(body->velocity, velocity);
    body->mass = mass;
    body->gravity = gravity;
    body->elasticity = elasticity;
    body->shape = shape;
    body->data = data;
    body->touch = touch;
    gfc_word_cpy(body->name, name);
}

Shape gf2d_body_to_shape(Body* a)
{
    Shape aS = { 0 };
    if (!a)return aS;
    gfc_shape_copy(&aS, *a->shape);
    gfc_shape_move(&aS, a->position);
    return aS;
}

Uint8 gf2d_body_body_collide(Body* a, Body* b)
{
    if ((!a) || (!b))
    {
        slog("missing body in collision check");
        return 0;
    }
    return gfc_shape_overlap(gf2d_body_to_shape(a), gf2d_body_to_shape(b));
}


void body_manager_close() {

    body_free_all();
    if (body_manager.body_list)
    {
        free(body_manager.body_list);
    }
    slog("body system closed");
}

void body_free_all() {
    int i;
    for (i = 0; i < body_manager.body_max; i++) {
        if (!body_manager.body_list[i].inuse)continue;        
        gf2d_body_clear(&body_manager.body_list[i]);
    }
}

void gf2d_body_manager_init(Uint32 max) 
{
    if (max <= 0)
    {
        slog("cannot inialize entity system: zero entities specified!");
        return;
    }
    body_manager.body_list = gfc_allocate_array(sizeof(Body), max);
    if (!body_manager.body_list) {
        slog("failed to initialize body system!");
        return;
    }
    body_manager.body_max = max;
    body_manager.body_def = sj_load("config/entities.def");
    atexit(body_manager_close);//like an on disable but for the whole prog
    slog("body system initialized");
}

void gf2d_body_from_config(Body* body, SJson* config)
{
    const char* str;
    if ((!body) || (!config)) return;
    str = sj_object_get_value_as_string(config, "name");
    if (str)gfc_line_cpy(body->name, str);
    sj_object_get_value_as_float(config, "gravity", &body->gravity);
    sj_object_get_value_as_uint8(config, "worldclip", &body->worldclip);
    sj_object_get_value_as_uint32(config, "team", &body->team);
    sj_value_as_vector2d(sj_object_get_value(config, "position"), &body->position);
    sj_value_as_vector2d(sj_object_get_value(config, "velocity"), &body->velocity);
    sj_object_get_value_as_float(config, "mass", &body->mass);
    sj_object_get_value_as_float(config, "elasticity", &body->elasticity);
    body->cliplayer = gf3d_cliplayers_from_config(sj_object_get_value(config, "cliplayer"));
    body->touchlayer = gf3d_cliplayers_from_config(sj_object_get_value(config, "touchlayer"));
}

/*eol@eof*/